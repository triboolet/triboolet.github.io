#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for mkdir
#include <sys/stat.h>
// for the 0755 const
#include <sys/types.h>

char *slice(const char *start, const char *end) {
  if (!start || !end || end < start)
    return NULL;

  size_t len = end - start;
  char *result = malloc(len + 1);
  if (!result)
    return NULL;

  strncpy(result, start, len);
  result[len] = '\0';
  return result;
}

int is_markdown_file(const char *fileName) {
  const char *dot = strrchr(fileName, '.');
  return dot && strcmp(dot, ".md") == 0;
}

void pandoc_md_to_html(const char *md_path, const char *html_path) {
  // Simply calls pandoc to convert the markdown file at md_path to an html one
  // at html_path
  char command[1024];
  snprintf(command, sizeof(command),
           "pandoc \"%s\" -f markdown -t html -o \"%s\" --css=static/theme.css",
           md_path, html_path);
  printf("Running %s\n", command);
  int ret = system(command);
  if (ret != 0) {
    perror("pandoc command failed");
    exit(EXIT_FAILURE);
  }
}

void write_index(const char **html_files, int count) {
  FILE *index = fopen("site/index.html", "w");
  if (!index) {
    perror("Cannot write site/index.html");
    exit(EXIT_FAILURE);
  }

  fprintf(index, "<!DOCTYPE html>\n<html>\n<head><title>My "
                 "Site</title></head>\n<body>\n<h1>My Blog</h1>\n<ul>\n");

  for (int i = 0; i < count; i++) {
    const char *pagePath = html_files[i];
    char *dot = strrchr(pagePath, '.');
    char *lastSlash = strrchr(pagePath, '/');
    char *name = slice(lastSlash + 1, dot);

    fprintf(index, "<li><a href=\"%s\">%s</a></li>\n", html_files[i], name);
  }

  fprintf(index, "</ul>\n</body>\n</html>\n");
  fclose(index);
}

int main() {
  const int MAX_FILES = 1000;
  char md_dir[256];
  char html_dir[256];
  snprintf(md_dir, sizeof(md_dir), "static/articles");
  snprintf(html_dir, sizeof(html_dir), "site/pages");

  DIR *dir = opendir(md_dir);
  // if the directory does not exist
  if (ENOENT == errno) {
    mkdir("site", 0755);
  }
  if (!dir) {
    perror("Problem when trying to open the site directory");
    return EXIT_FAILURE;
  }

  struct dirent *entry;
  const char *html_files[MAX_FILES];
  int numberOfGeneratedFiles = 0;

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG && is_markdown_file(entry->d_name)) {
      printf("File : %s\n", entry->d_name);
      char md_path[1024];
      char html_path[1024];
      snprintf(md_path, sizeof(md_path), "%s/%s", md_dir, entry->d_name);

      // Strip .md and append .html
      char *dot = strrchr(entry->d_name, '.');
      size_t basename_len = dot - entry->d_name;
      char base[256];
      strncpy(base, entry->d_name, basename_len);
      base[basename_len] = '\0';

      snprintf(html_path, sizeof(html_path), "%s/%s.html", html_dir, base);
      pandoc_md_to_html(md_path, html_path);

      // add the path to the list of generated html files for the index
      // we need to trim the "site" part of the string, as index.html lives in
      // "site"
      char *sitePrefix = strchr(html_path, '/');
      char *htmlPathNoPrefix = sitePrefix + 1;
      char pathToHTML[256];
      snprintf(pathToHTML, sizeof(pathToHTML), "%s", htmlPathNoPrefix);
      html_files[numberOfGeneratedFiles++] = strdup(pathToHTML);
    }
  }
  for (int i = 0; i < numberOfGeneratedFiles; i++) {
    printf("%s\n", html_files[i]);
  }

  write_index(html_files, numberOfGeneratedFiles);

  closedir(dir);
  return 0;
}
