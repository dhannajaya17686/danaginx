#include "request.h" // Include the header file where the HttpRequest struct is defined.
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
