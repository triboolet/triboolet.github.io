#pragma once

const char *trim_spaces(const char *line);
char *replaceFirstOccurence(const char *string, const char *substring,
                            const char *replacementString);
int fixCSSRelativePath(char *htmlFilePath, char *wrongCSSFilePath,
                       char *rightCSSFilePath);
