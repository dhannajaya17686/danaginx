#include "../include/routes.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Route *initRoute(const char *key, const char *value) {
  struct Route *temp = (struct Route *)malloc(sizeof(struct Route));
  if (!temp) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  temp->key = strdup(key);
  temp->value = strdup(value);

  temp->left = temp->right = NULL;
  return temp;
}

void inorder(struct Route *root) {
  if (root != NULL) {
    inorder(root->left);
    printf("%s -> %s \n", root->key, root->value);
    inorder(root->right);
  }
}

struct Route *addRoute(struct Route *root, const char *key, const char *value) {
  if (root == NULL) {
    return initRoute(key, value);
  }

  int cmp = strcmp(key, root->key);
  if (cmp == 0) {
    printf("============ WARNING ============\n");
    printf("A Route For \"%s\" Already Exists\n", key);
  } else if (cmp > 0) {
    root->right = addRoute(root->right, key, value);
  } else {
    root->left = addRoute(root->left, key, value);
  }

  return root; // Ensure the root is returned
}

struct Route *search(struct Route *root, const char *key) {
  if (root == NULL) {
    return NULL;
  }

  int cmp = strcmp(key, root->key);
  if (cmp == 0) {
    return root;
  } else if (cmp > 0) {
    return search(root->right, key);
  } else {
    return search(root->left, key);
  }
}

// Free the allocated memory
void freeRoutes(struct Route *root) {
  if (root == NULL)
    return;
  freeRoutes(root->left);
  freeRoutes(root->right);
  free(root->key);
  free(root->value);
  free(root);
}
