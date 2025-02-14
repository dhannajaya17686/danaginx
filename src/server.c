#include "../include/server.h"
#include "../include/request.h"
#include "../include/response.h"
#include "../include/routes.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Constructor function for initializing the server.
struct Server server_Constructor(int domain, int port, int service,
                                 int protocol, int backlog, u_long interface,
                                 void (*launch)(struct Server *server)) {
  struct Server server;

  server.domain = domain;
  server.service = service;
  server.port = port;
  server.protocol = protocol;
  server.backlog = backlog;
  server.routes = NULL; // Initialize routes tree
  scan_directory(&server.routes, "templates", "");
  server.address.sin_family = domain;
  server.address.sin_port = htons(port);
  server.address.sin_addr.s_addr = htonl(interface);

  server.socket = socket(domain, service, protocol);
  if (server.socket < 0) {
    perror("Failed to initialize socket");
    exit(EXIT_FAILURE);
  }

  if (bind(server.socket, (struct sockaddr *)&server.address,
           sizeof(server.address)) < 0) {
    perror("Failed to bind socket");
    exit(EXIT_FAILURE);
  }

  if (listen(server.socket, backlog) < 0) {
    perror("Failed to start listening");
    exit(EXIT_FAILURE);
  }

  server.launch = launch;
  return server;
}

void scan_directory(struct Route **root, const char *base_path,
                    const char *prefix) {
  struct dirent *entry;
  DIR *dir = opendir(base_path);
  if (!dir) {
    perror("Failed to open directory");
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    // Ignore hidden files (like .git, .DS_Store)
    if (entry->d_name[0] == '.') {
      continue;
    }

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);

    struct stat path_stat;
    stat(full_path, &path_stat);

    if (S_ISDIR(path_stat.st_mode)) {
      // Recursively scan subdirectories
      char new_prefix[512];
      snprintf(new_prefix, sizeof(new_prefix), "%s/%s", prefix, entry->d_name);
      scan_directory(root, full_path, new_prefix);
    } else {
      // Only add .html files to routes
      if (strstr(entry->d_name, ".html") != NULL) {
        // Construct route path (remove .html for clean URLs)
        char route_path[512];
        snprintf(route_path, sizeof(route_path), "%s/%s", prefix,
                 entry->d_name);

        // Remove ".html" for clean routes
        route_path[strlen(route_path) - 5] = '\0';

        // Special case: If the file is index.html, map "/" instead of "/index"
        if (strcmp(route_path, "/index") == 0) {
          strcpy(route_path, "/");
        }

        *root = addRoute(*root, route_path, full_path);
        printf("Added Route: %s -> %s\n", route_path, full_path);
      }
    }
  }

  closedir(dir);
}

// Function to handle incoming connections and serve HTTP responses.
void launch(struct Server *server) {
  char buffer[BUFFER_SIZE];

  while (1) {
    printf("=== WAITING FOR CONNECTION === \n");

    socklen_t addrlen = sizeof(server->address);

    int new_socket =
        accept(server->socket, (struct sockaddr *)&server->address, &addrlen);
    if (new_socket < 0) {
      perror("Failed to accept connection");
      continue;
    }

    ssize_t bytesRead = read(new_socket, buffer, BUFFER_SIZE - 1);
    if (bytesRead < 0) {
      perror("Error reading buffer");
      close(new_socket);
      continue;
    }

    buffer[bytesRead] = '\0';

    struct HttpRequest request = parse_request(buffer);
    printf("Method: %s\n", request.method);
    printf("Path: %s\n", request.path);
    printf("Headers:\n%s\n", request.headers);
    // Find requested file
    // Lookup route
    struct Route *destination = search(server->routes, request.path);
    char *response_data = NULL;
    char template[256] = "";
    char *content_type = "text/html"; // Default content type

    if (destination != NULL) {
      snprintf(template, sizeof(template), "%s", destination->value);
    } else {
      snprintf(template, sizeof(template), "templates%.*s",
               (int)(sizeof(template) - 10), request.path);
    }

    struct FileData fileData = read_file(template);
    char http_header[4096];

    if (fileData.size > 0) {
      // Determine content type
      content_type = get_mime_type(template);
      // Send HTTP response header
      snprintf(http_header, sizeof(http_header),
               "HTTP/1.1 200 OK\r\n"
               "Content-Type: %s\r\n"
               "Content-Length: %zu\r\n"
               "\r\n",
               content_type, fileData.size);

      send(new_socket, http_header, strlen(http_header), 0);
      send(new_socket, fileData.data, fileData.size, 0); // Send file content
      free(fileData.data);
    } else {
      // Send 404 response if file not found
      snprintf(http_header, sizeof(http_header),
               "HTTP/1.1 404 Not Found\r\n\r\n");
      send(new_socket, http_header, strlen(http_header), 0);
    }
    close(new_socket);
  }
}

char *get_mime_type(char *path) {
  const char *dot = strrchr(path, '.'); // Get file extension
  if (!dot)
    return "application/octet-stream"; // Default binary data

  if (strcmp(dot, ".html") == 0)
    return "text/html";
  if (strcmp(dot, ".css") == 0)
    return "text/css";
  if (strcmp(dot, ".js") == 0)
    return "application/javascript";
  if (strcmp(dot, ".png") == 0)
    return "image/png";
  if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
    return "image/jpeg";
  if (strcmp(dot, ".gif") == 0)
    return "image/gif";
  if (strcmp(dot, ".svg") == 0)
    return "image/svg+xml";
  if (strcmp(dot, ".json") == 0)
    return "application/json";
  if (strcmp(dot, ".ico") == 0)
    return "image/x-icon";

  return "application/octet-stream"; // Default for unknown types
}
