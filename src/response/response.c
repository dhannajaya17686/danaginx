#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *render_static_file(char *fileName) {
  FILE *file = fopen(fileName, "rb");

  if (file == NULL) {
    return NULL;
  } else {
    printf("%s does exist \n", fileName);
  }

  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *temp = malloc(sizeof(char) * (fsize + 1));
  char ch;
  int i = 0;
  while ((ch = fgetc(file)) != EOF) {
    temp[i] = ch;
    i++;
  }
  fclose(file);
  return temp;
}

struct FileData read_file(const char *filename) {
  struct FileData fileData = {NULL, 0};

  FILE *file = fopen(filename, "rb"); // Open in binary mode
  if (!file) {
    return fileData; // Return empty if file not found
  }

  fseek(file, 0, SEEK_END);
  fileData.size = ftell(file);
  rewind(file);

  fileData.data = (char *)malloc(fileData.size);
  if (!fileData.data) {
    fclose(file);
    fileData.size = 0;
    return fileData;
  }

  fread(fileData.data, 1, fileData.size, file);
  fclose(file);

  return fileData;
}
