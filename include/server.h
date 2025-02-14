#ifndef server_h
#define server_h

#include "routes.h"
#include <netinet/in.h>

#define BUFFER_SIZE 16000

struct Server {
  int domain;
  int port;
  int service;
  int protocol;
  int backlog;
  u_long interface;

  int socket;
  struct sockaddr_in address;

  struct Route *routes; // Added field for routing

  void (*launch)(struct Server *server);
};

struct Server server_Constructor(int domain, int port, int service,
                                 int protocol, int backlog, u_long interface,
                                 void (*launch)(struct Server *server));
void launch(struct Server *server);
char *get_mime_type(char *path);
void scan_directory(struct Route **root, const char *base_path,
                    const char *prefix);

#endif
