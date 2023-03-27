#include "cat_src.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isflag(char *str) { return str[0] == '-'; }

void init_options(Options *options) {
  options->b_f = 0;
  options->e_f = 0;
  options->n_f = 0;
  options->s_f = 0;
  options->t_f = 0;
  options->v_f = 0;
  options->f_error = 0;
}

void flag_sn(unsigned char past, unsigned char cur, int *empty_line,
             int *stabil, int *line) {
  if (past == '\n' && cur != '\n') {
    printf("%6.d\t", *line);
    (*line)++;
  }
  if (past == '\n' && cur == '\n' && *empty_line == 0) {
    printf("%6.d\t", *line);
    (*line)++;
    printf("\n");
    *empty_line = 1;
    *stabil = 0;
  }
  if (past == '\n' && cur != '\n') {
    *stabil = 1;
    *empty_line = 0;
  }
  if (past == '\n' && cur == '\n' && *empty_line == 1) {
    *stabil = 0;
  }
}
void flag_se(unsigned char past, unsigned char cur, int *empty_line,
             int *stabil) {
  if (past != '\n' && cur == '\n') {
    printf("$");
  }
  if (past == '\n' && cur == '\n' && *empty_line == 0) {
    printf("$\n");
    *empty_line = 1;
    *stabil = 0;
  }
  if (past == '\n' && cur != '\n') {
    *stabil = 1;
    *empty_line = 0;
  }
  if (past == '\n' && cur == '\n' && *empty_line == 1) {
    *stabil = 0;
  }
}
void flag_s(unsigned char past, unsigned char cur, int *empty_line,
            int *stabil) {
  if (past == '\n' && cur == '\n' && *empty_line == 0) {
    printf("\n");
    *empty_line = 1;
    *stabil = 0;
  }
  if (past == '\n' && cur != '\n') {
    *stabil = 1;
    *empty_line = 0;
  }
  if (past == '\n' && cur == '\n' && *empty_line == 1) {
    *stabil = 0;
  }
}
void flag_b(unsigned char past, unsigned char cur, int *line) {
  if (past == '\n' && cur != '\n') {
    printf("%6.d\t", *line);
    (*line)++;
  }
}
void flag_n(unsigned char sym, int *line) {
  if (sym == '\n') {
    printf("%6.d\t", *line);
    (*line)++;
  }
}
void flag_t(unsigned char sym, int *stabil) {
  if (sym == '\t') {
    printf("^I");
    *stabil = 0;
  }
}
void flag_e(unsigned char sym) {
  if (sym == '\n') {
    printf("$");
  }
}
void flag_v(unsigned char sym, int *stabil) {
  if ((int)sym <= 31 && sym != '\n' && sym != '\t') {
    printf("^%c", (char)((int)sym + 64));
    *stabil = 0;

  } else if ((int)sym == 127) {
    printf("^?");
    *stabil = 0;

  } else if ((int)sym > 127) {
    printf("M-^%c", (char)((int)sym - 64));
    *stabil = 0;
  }
}
void extract_f(Options *options, char *flags) {
  if (flags[1] == '-') {
    if (strcmp(flags, "--number-nonblank") == 0) {
      options->b_f = 1;
      options->n_f = 0;
    } else if (strcmp(flags, "--number") == 0) {
      if (options->b_f == 0) {
        options->n_f = 1;
      }
    } else if (strcmp(flags, "--squeeze-blank") == 0) {
      options->s_f = 1;
    } else {
      options->f_error = 1;
    }
  } else {
    simple_option(options, flags);
  }
}

void simple_option(Options *options, char *flags) {
  int flags_count = strlen(flags) - 1;
  for (long unsigned int i = 1; i < strlen(flags); i++) {
    char f = flags[i];
    if (f == 'b') {
      options->b_f = 1;
      options->n_f = 0;
    } else if (f == 'e') {
      options->e_f = 1;
      options->v_f = 1;
    } else if (f == 'E') {
      options->e_f = 1;
    } else if (f == 'n') {
      if (options->b_f == 0) {
        options->n_f = 1;
      }
    } else if (f == 's') {
      options->s_f = 1;
    } else if (f == 't') {
      options->t_f = 1;
      options->v_f = 1;
    } else if (f == 'T') {
      options->t_f = 1;
    } else if (f == 'v') {
      options->v_f = 1;
    } else {
      options->f_error = 1;
    }
  }
  if (flags_count == 0) {
    options->f_error = 1;
  }
}

void print_sym(char *file_name, Options options) {
  FILE *fin;
  if ((fin = fopen(file_name, "r")) != NULL) {
    unsigned char past = '\n';
    unsigned char cur = fgetc(fin);
    int line = 1;
    int empty_line = 0;
    while (!feof(fin)) {
      int stabil = 1;
      if (options.b_f == 1) {
        flag_b(past, cur, &line);
      }
      if (options.n_f == 1 && options.s_f == 0) {
        flag_n(past, &line);
      } else if (options.n_f == 1 && options.s_f != 0) {
        flag_sn(past, cur, &empty_line, &stabil, &line);
      }
      if (options.e_f == 1 && options.s_f == 0) {
        flag_e(cur);
      } else if (options.e_f == 1 && options.s_f != 0) {
        flag_se(past, cur, &empty_line, &stabil);
      }
      if (options.s_f == 1 && options.e_f == 0 && options.n_f == 0) {
        flag_s(past, cur, &empty_line, &stabil);
      }
      if (options.t_f == 1) {
        flag_t(cur, &stabil);
      }
      if (options.v_f == 1) {
        flag_v(cur, &stabil);
      }
      if (stabil == 1) {
        printf("%c", cur);
      }
      past = cur;
      cur = getc(fin);
    }
    fclose(fin);
  } else {
    fprintf(stderr, "%s", "cat: No such file or directory\n");
  }
}