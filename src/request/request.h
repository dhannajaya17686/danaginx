#ifndef request_h
#define server_h

#define MAX_REQUEST_SIZE 2048

#define MAX_HEADERS 20
#define MAX_HEADER_SIZE 1024

struct HttpRequestHeaders {
  char host[MAX_HEADER_SIZE];
  char user_agent[MAX_HEADER_SIZE];
  char accept[MAX_HEADER_SIZE];
  char accept_language[MAX_HEADER_SIZE];
  char accept_encoding[MAX_HEADER_SIZE];
  char sec_gpc[MAX_HEADER_SIZE];
  char connection[MAX_HEADER_SIZE];
  char upgrade_insecure_requests[MAX_HEADER_SIZE];
  char sec_fetch_dest[MAX_HEADER_SIZE];
  char sec_fetch_mode[MAX_HEADER_SIZE];
  char sec_fetch_site[MAX_HEADER_SIZE];
  char sec_fetch_user[MAX_HEADER_SIZE];
  char priority[MAX_HEADER_SIZE];
};

struct HttpRequest {
  char method[8];
  char path[256];
  char headers[1024];
};

struct HttpRequest parse_request(const char *request);

#endif // !request_h
