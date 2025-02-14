#include "../include/server.h"
#include <sys/socket.h>

int main() {
  // CREATES A SERVER INSTANCE HAS A FUNCTION CALLED SERVER CONSTRUCOR WHICH
  // RETURNS A STRUCT TYPE INSTANCE CALLED Server
  struct Server server = server_Constructor(
      AF_INET,     // Address family: Ipv4 (domain)
      8080,        // poer number : 8080
      SOCK_STREAM, // socket type: Stream socket(TCP)
      0,           // Protocol : Default
      10,          // Maximumm number of pending connections in the que
      INADDR_ANY,  // bind to any available network interface
      launch       // function pointer to the server launch functionality
  );
  server.launch(&server); // calling the launch function to start the server
                          // Initialize routes
  return 0;
}
