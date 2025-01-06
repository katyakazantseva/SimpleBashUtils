#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 4096

int main(int argc, char *argv[]) {
  grep_options options = {0};

  parse_options(argc, argv, &options);

  regex_t regex;
  compile_pattern(&regex, options.pattern, options.i);

  search_file(&regex, &options, argc, argv);
  regfree(&regex);

  return 0;
}

void parse_options(int argc, char *argv[], grep_options *options) {
  int opt;
  int cou = 0;
  int option_index;
  struct option longOptions[] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "e:ivclnhsof:", longOptions,
                            &option_index)) != -1) {
    switch (opt) {
      case 'e':
        cou++;
        if (cou > 1) {
          strcat(options->pattern, "|");
          strcat(options->pattern, optarg);
        }
        if (cou == 1) {
          strcat(options->pattern, optarg);
        }
        options->e += 1;
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        fprintf(stderr, "Error flag\n");
    }
  }
  patterns(options, argv, argc);
}

void compile_pattern(regex_t *regex, char *pattern, int i) {
  int flags = REG_EXTENDED;
  if (i) {
    flags |= REG_ICASE;
  }
  int ret = regcomp(regex, pattern, flags);
  if (ret != 0) {
    fprintf(stderr, "Error compile pattern\n");
    regfree(regex);
    exit(1);
  }
}

void search_file(regex_t *regex, grep_options *options, int argc,
                 char *argv[]) {
  int filen = 0;
  if (!options->f && !options->e)
    filen = optind + 1;
  else
    filen = optind;
  if (filen + 1 != argc) options->count = 1;
  while (filen < argc) {
    FILE *file = fopen(argv[filen], "r");
    if (file == NULL) {
      if (!options->s) {
        fprintf(stderr, "Error opening file\n");
        regfree(regex);
        exit(1);
      } else {
        filen++;
        continue;
      }
    }
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int matches = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
      line_number++;
      regmatch_t match;
      int ret = regexec(regex, line, 1, &match, 0);
      if (ret == 0) {
        matches++;
        conform(matches, options, argv, filen, line_number, line, match);

      } else if (options->v) {
        if (options->count == 1) printf("%s:", argv[filen]);
        printf("%s", line);
        if (strchr(line, '\n') == NULL) printf("\n");
      }
    }
    if (options->c) {
      if (options->count == 1) printf("%s:", argv[filen]);
      printf("%d\n", matches);
    }
    filen++;
    if (file != NULL) fclose(file);
  }
}

void f_flag(char *argv[], grep_options *options) {
  FILE *ff = fopen(argv[optind - 1], "r");
  char line1[MAX_LINE_LENGTH];
  char pat[MAX_LINE_LENGTH] = {0};
  int i = 0;
  while (fgets(line1, MAX_LINE_LENGTH, ff) != NULL) {
    i++;
    char *arc = strchr(line1, '\n');
    if (arc != NULL) {
      line1[arc - line1] = '\0';
    }
    if (i > 1) {
      strcat(pat, "|");
      if (line1[0] == '\0') {
        strcat(pat, ".");
      }
    }
    strcat(pat, line1);
  }
  // printf("%s", pat);
  strcat(options->pattern, pat);
}

int conform(int matches, grep_options *options, char *argv[], int filen,
            int line_number, char *line, regmatch_t match) {
  if (!options->c && !options->h && !options->o && !options->v && !options->l) {
    if (options->count == 1) printf("%s:", argv[filen]);
    if (options->n) printf("%d:", line_number);
    printf("%s", line);
    if (strchr(line, '\n') == NULL) printf("\n");
  }
  if (options->h) printf("%s", line);
  if (options->o) {
    if (options->count == 1) printf("%s:", argv[filen]);
    printf("%.*s\n", (int)match.rm_eo - (int)match.rm_so, &line[match.rm_so]);
  }
  if (options->l) {
    if (matches == 1) printf("%s\n", argv[filen]);
  }
  return matches;
}

void patterns(grep_options *options, char *argv[], int argc) {
  if (!options->e) {
    if (options->f) {
      f_flag(argv, options);
    } else if (optind < argc) {
      strcat(options->pattern, argv[optind]);
    } else {
      fprintf(stderr, "Error pattern\n");
      exit(1);
    }
  }
}
