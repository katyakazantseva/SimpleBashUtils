#ifndef GREP
#define GREP

#include <regex.h>

typedef struct {
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int e;
  int f;
  int o;
  int count;
  char pattern[4096];
} grep_options;

void f_flag(char *argv[], grep_options *options);
void parse_options(int argc, char *argv[], grep_options *options);
void compile_pattern(regex_t *regex, char *pattern, int i);
void search_file(regex_t *regex, grep_options *options, int argc, char *argv[]);
int conform(int matches, grep_options *options, char *argv[], int filen,
            int line_number, char *line, regmatch_t match);
void patterns(grep_options *options, char *argv[], int argc);

#endif
