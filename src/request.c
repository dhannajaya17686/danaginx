#include "../include/request.h" // Include the header file where the HttpRequest struct is defined.
#include <stdio.h>  // Standard input/output functions.
#include <stdlib.h> // Standard library functions.
#include <string.h> // String manipulation functions (e.g., memset, strncpy, strchr).

// Function to parse an incoming HTTP request string and extract key
// information.
struct HttpRequest parse_request(const char *request) {
  struct HttpRequest
      httpRequest; // Declare an HttpRequest struct to store parsed data.

  // Initialize the struct to zero to prevent garbage values.
  memset(&httpRequest, 0, sizeof(struct HttpRequest));

  // Extract the HTTP method (GET, POST, etc.) and the requested path (e.g.,
  // "/index.html").
  sscanf(request, "%s %s", httpRequest.method, httpRequest.path);

  // Find the start of the headers (after the first line, marked by a newline
  // '\n').
  char *header_begin = strchr(request, '\n');

  // If headers exist, copy them into the headers field of the HttpRequest
  // struct.
  if (header_begin != NULL) {
    strncpy(httpRequest.headers, header_begin + 1,
            sizeof(httpRequest.headers) - 1);
  }

  return httpRequest; // Return the parsed HttpRequest structure.
}

void parse_headers(const char *raw_headers,
                   struct HttpRequestHeaders *headers) {
  memset(headers, 0, sizeof(struct HttpRequestHeaders));

  char *headers_copy = strdup(raw_headers);
  char *line = strtok(headers_copy, "\n");

  while (line) {
    if (strncasecmp(line, "Host:", 5) == 0) {
      strncpy(headers->host, line + 6, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "User-Agent:", 11) == 0) {
      strncpy(headers->user_agent, line + 12, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Accept:", 7) == 0) {
      strncpy(headers->accept, line + 8, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Accept-Language:", 16) == 0) {
      strncpy(headers->accept_language, line + 17, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Accept-Encoding:", 16) == 0) {
      strncpy(headers->accept_encoding, line + 17, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Sec-GPC:", 8) == 0) {
      strncpy(headers->sec_gpc, line + 9, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Connection:", 10) == 0) {
      strncpy(headers->connection, line + 11, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Upgrade-Insecure-Requests:", 26) == 0) {
      strncpy(headers->upgrade_insecure_requests, line + 27, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Sec-Fetch-Dest:", 15) == 0) {
      strncpy(headers->sec_fetch_dest, line + 16, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Sec-Fetch-Mode:", 15) == 0) {
      strncpy(headers->sec_fetch_mode, line + 16, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Sec-Fetch-Site:", 15) == 0) {
      strncpy(headers->sec_fetch_site, line + 16, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Sec-Fetch-User:", 15) == 0) {
      strncpy(headers->sec_fetch_user, line + 16, MAX_HEADER_SIZE);
    } else if (strncasecmp(line, "Priority:", 9) == 0) {
      strncpy(headers->priority, line + 10, MAX_HEADER_SIZE);
    }
    line = strtok(NULL, "\n");
  }
  free(headers_copy);
}

void print_headers(struct HttpRequestHeaders *headers) {
  printf("Host: %s\n", headers->host);
  printf("User-Agent: %s\n", headers->user_agent);
  printf("Accept: %s\n", headers->accept);
  printf("Accept-Language: %s\n", headers->accept_language);
  printf("Accept-Encoding: %s\n", headers->accept_encoding);
  printf("Sec-GPC: %s\n", headers->sec_gpc);
  printf("Connection: %s\n", headers->connection);
  printf("Upgrade-Insecure-Requests: %s\n", headers->upgrade_insecure_requests);
  printf("Sec-Fetch-Dest: %s\n", headers->sec_fetch_dest);
  printf("Sec-Fetch-Mode: %s\n", headers->sec_fetch_mode);
  printf("Sec-Fetch-Site: %s\n", headers->sec_fetch_site);
  printf("Sec-Fetch-User: %s\n", headers->sec_fetch_user);
  printf("Priority: %s\n", headers->priority);
}
