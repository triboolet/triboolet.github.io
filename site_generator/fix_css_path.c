#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// for isspace()
#include <ctype.h>

#include "fix_css_path.h"

const char *trim_spaces(const char *line) {
  while (*line && isspace(*line)) {
    line++;
  }
  return line;
}

char *replaceFirstOccurence(const char *string, const char *substring,
                            const char *replacementString) {
  const char *pos = strstr(string, substring);
  if (!pos) {
    return strdup(string);
  }

  size_t before_len = pos - string;
  size_t needle_len = strlen(substring);
  size_t replacement_len = strlen(replacementString);
  size_t new_len = before_len + replacement_len + strlen(pos + needle_len);

  char *result = (char *)malloc(new_len + 1);
  if (!result)
    return NULL;

  memcpy(result, string, before_len);
  memcpy(result + before_len, replacementString, replacement_len);
  strcpy(result + before_len + replacement_len, pos + needle_len);

  return result;
}

int fixCSSRelativePath(char *htmlFilePath, char *wrongCSSFilePath,
                       char *rightCSSFilePath) {
  /*
   * When generating html files with CSS, pandoc will set the CSS file path as
   * the one it was given when called. The CSS file path should however be set
   * relative to the html's directory. This is why this function takes the path
   * to an html file and replaces it with the given CSS file path.
   */
  FILE *file = fopen(htmlFilePath, "r+");
  if (!file) {
    perror("Couldn't open file");
    return EXIT_FAILURE;
  }

  char *writeBuffer = NULL;
  size_t size = 0;
  FILE *fixedFile = open_memstream(&writeBuffer, &size);
  if (!fixedFile) {
    perror("Couldn't open memory stream");
    return EXIT_FAILURE;
  }
  char buffer[1024];
  const char *styleSheetLinkLine = "<link rel=\"stylesheet\" href=";

  while (fgets(buffer, sizeof(buffer), file)) {
    if (strncmp(styleSheetLinkLine, trim_spaces(buffer),
                strlen(styleSheetLinkLine)) == 0) {
      if (fputs(
              replaceFirstOccurence(buffer, wrongCSSFilePath, rightCSSFilePath),
              fixedFile) == EOF) {
        perror("Error while writing into temp file");
        return EXIT_FAILURE;
      }
    } else {
      if (fputs(buffer, fixedFile) == EOF) {
        perror("Error while writing into temp file");
        return EXIT_FAILURE;
      }
    }
  }
  fclose(fixedFile);

  // Copy the memory buffer content into the actual file
  rewind(file);
  fwrite(writeBuffer, 1, size, file);
  fflush(file);

  free(writeBuffer);
  fclose(file);
  return 0;
}
