#ifndef routes_h
#define routes_h

#include <stddef.h>
struct FileData {
  char *data;
  size_t size;
};

struct FileData read_file(const char *filename);
char *render_static_file(char *fileName);

#endif
