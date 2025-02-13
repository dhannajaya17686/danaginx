#include "server.h" // Include the custom header file that defines the Server structure.
#include <stdio.h>  // Include standard input/output functions.
#include <stdlib.h> // Include standard library functions such as exit().
#include <string.h> // Include string handling functions.
#include <unistd.h> // Include POSIX API for system calls like read() and write().

// Constructor function for initializing the server.
struct Server server_Constructor(int domain, int port, int service,
                                 int protocol, int backlog, u_long interface,
                                 void (*launch)(struct Server *server)) {
  struct Server server; // Create a Server struct instance.

  // Assign server properties based on provided parameters.
  server.domain = domain;
  server.service = service; // Basically mean how data is transferred via the
                            // network tcp or udp
  server.port = port;
  server.protocol = protocol;
  server.backlog = backlog;

  // Configure the server address structure.
  server.address.sin_family = domain; // Address family (IPv4 or IPv6).
  server.address.sin_port =
      htons(port); // Convert port number to network byte order.
  server.address.sin_addr.s_addr =
      htonl(interface); // Convert IP address to network byte order.

  // Create a socket with the given domain, service, and protocol.
  server.socket = socket(domain, service, protocol);
  if (server.socket < 0) {
    perror(
        "Failed to initialize/connect to socket...\n"); // Print error if socket
                                                        // creation fails.
    exit(EXIT_FAILURE); // Exit program if socket creation fails.
  }

  // Bind the socket to the specified IP and port.
  if (bind(server.socket, (struct sockaddr *)&server.address,
           sizeof(server.address)) < 0) {
    perror("Failed to bind socket...\n"); // Print error if binding fails.
    exit(EXIT_FAILURE);                   // Exit program if binding fails.
  }

  // Start listening for incoming connections with a given backlog size.
  if (listen(server.socket, server.backlog) < 0) {
    perror("Failed to start listening...\n"); // Print error if listen fails.
    exit(EXIT_FAILURE); // Exit program if listening fails.
  }

  server.launch = launch; // Assign the launch function pointer to the server.
  return server;          // Return the constructed server instance.
}

// Function to handle incoming connections and serve HTTP responses.
void launch(struct Server *server) {
  char buffer[BUFFER_SIZE]; // Buffer to store incoming data.

  while (1) { // Infinite loop to keep accepting client connections.
    printf("=== WAITING FOR CONNECTION === \n");

    int addrlen =
        sizeof(server->address); // Get the size of the address structure.

    // Accept a new client connection.
    int new_socket = accept(server->socket, (struct sockaddr *)&server->address,
                            (socklen_t *)&addrlen);
    // TODO: make a parser to parse incoming requests and make proper responses
    // Read the incoming request into the buffer.
    ssize_t bytesRead = read(new_socket, buffer, BUFFER_SIZE - 1);

    if (bytesRead >= 0) {
      buffer[bytesRead] = '\0'; // Null-terminate the received string.
      puts(buffer);             // Print the received HTTP request.
    } else {
      perror("Error reading buffer...\n"); // Print error if reading fails.
    }

    // Define a basic HTTP response with an HTML message.
    char *response = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n"
                     "<html>\r\n"
                     "<head>\r\n"
                     "<title>Testing Basic HTTP-SERVER</title>\r\n"
                     "</head>\r\n"
                     "<body>\r\n"
                     "Hello, Ahmed!\r\n"
                     "</body>\r\n"
                     "</html>\r\n";

    // Send the response back to the client.
    write(new_socket, response, strlen(response));
    // TODO: making a system where the response is injected based on the request

    // Close the client connection.
    close(new_socket);
  }
}
