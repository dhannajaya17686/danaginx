
#ifndef ROUTES_H
#define ROUTES_H

#include <stdlib.h>
#include <string.h>

struct Route {
  char *key;
  char *value;
  struct Route *left, *right;
};

struct Route *initRoute(const char *key, const char *value);
struct Route *addRoute(struct Route *root, const char *key, const char *value);
struct Route *search(struct Route *root, const char *key);
void inorder(struct Route *root);
void freeRoutes(struct Route *root); // Added for proper memory cleanup

#endif // ROUTES_H
