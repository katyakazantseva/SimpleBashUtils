#include "s21_cat.h"

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  Flags flags = {0, 0, 0, 0, 0, 0, 0, 0};

  ReadFlags(argc, argv, &flags);

  if (flags.b) {
    flags.n = 0;
  }

  Args(argv, flags, argc);

  return 0;
}

void ReadFlags(int argc, char *argv[], Flags *flags) {
  int opt;
  int option_index;
  struct option longOptions[] = {{"number-nonblank", 0, 0, 'b'},
                                 {"number", 0, 0, 'n'},
                                 {"squeeze-blank", 0, 0, 's'},
                                 {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "+bevEnstT", longOptions,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        flags->b = 1;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'E':
        flags->E = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      case 'T':
        flags->T = 1;
        break;
      default:
        fprintf(stderr, "error");
        exit(1);
    }
  }
}

void Args(char *argv[], Flags flags, int argc) {
  while (optind < argc) {
    FILE *file = fopen(argv[optind], "r");
    if (file != NULL) {
      int cur;
      int str_count = 0;
      int pred = '\n';
      int squeeze = 0;
      while ((cur = fgetc(file)) != EOF) {
        if (flags.b && !flags.n) {
          if (cur != '\n') {
            if (pred == '\n') printf("%6d\t", ++str_count);
          }
        }
        if (flags.s) {
          if (cur == '\n' && pred == '\n') squeeze += 1;
          if (cur != '\n') squeeze = 0;
        }
        if (squeeze < 2) {
          if (flags.n) {
            if (pred == '\n') printf("%6d\t", ++str_count);
          }
          if (flags.E && cur == '\n') printf("$");
          if (flags.e && cur == '\n') printf("$");
          if (flags.T && cur == '\t') printf("^I");
          if (flags.t && cur == '\t') printf("^I");
          if (flags.v && cur != '\t' && cur != '\n' && cur <= 31) {
            printf("^%c", cur + 64);
          }
          if (flags.v && cur != '\t' && cur != '\n' && cur == 127) {
            printf("^%c", cur - 64);
          } else {
            if (!(flags.t && cur == '\t') && !(flags.T && cur == '\t') &&
                !(flags.v && cur != '\t' && cur != '\n' && cur <= 31) &&
                !(flags.v && cur != '\t' && cur != '\n' && cur == 127)) {
              printf("%c", cur);
            }
          }
        }
        pred = cur;
      }
    } else {
      printf("No such file or directory\n");
    }
    optind++;
    if (file != NULL) fclose(file);
  }
}
