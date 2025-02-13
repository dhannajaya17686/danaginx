#ifndef request_h
#define server_h

#define MAX_REQUEST_SIZE 2048

struct HttpRequest {
  char method[8];
  char path[256];
  char headers[1024];
};

struct HttpRequest parse_request(const char *request);

#endif // !request_h
