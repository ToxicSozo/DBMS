#include "../include/json_reader.h"

char* load_JSON_file(const char *fname) {
  FILE *fp = fopen(fname, "rb");

  if (!fp) {
    printf("Ошибка: не удалось открыть файл.\n");
    exit(1);
  }
  
  int64_t file_lenght = 0;

  fseek(fp, 0, SEEK_END);
  file_lenght = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *content = calloc(file_lenght + 1, sizeof(char));

  fread(content, 1, file_lenght, fp);
  content[file_lenght] = '\0';

  fclose(fp);
  
  return content;
}