#include "grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void init_options(Cmd* cmd) {
  cmd->opt_e = 0;
  cmd->opt_i = 0;
  cmd->opt_v = 0;
  cmd->opt_c = 0;
  cmd->opt_l = 0;
  cmd->opt_n = 0;
  cmd->opt_h = 0;
  cmd->opt_s = 0;
  cmd->opt_f = 0;
  cmd->opt_o = 0;
  cmd->opt_err = 0;
  cmd->arg_err = 0;
}

int check_low(char* line, char** patterns, int p_cnt) {
  int match = 0;
  char* cp_line = malloc((strlen(line) + 1) * sizeof(char));
  strcpy(cp_line, line);
  for (size_t i = 0; i < strlen(cp_line); i++) {
    if (cp_line[i] >= 65 && cp_line[i] <= 90) {
      cp_line[i] += 32;
    }
  }
  for (int i = 0; i < p_cnt; i++) {
    for (size_t j = 0; j < strlen(patterns[i]); j++) {
      if (patterns[i][j] >= 65 && patterns[i][j] <= 90) {
        patterns[i][j] += 32;
      }
    }
    regex_t regex;
    int reti = regcomp(&regex, patterns[i], 0);
    reti = regexec(&regex, cp_line, 0, NULL, 0);
    if (!reti) {
      match++;
    }
    regfree(&regex);
  }
  free(cp_line);
  return match;
}

int find_match(char** patterns, char* line, int p_num) {
  int match = 0;
  for (int i = 0; i < p_num; i++) {
    regex_t regex;
    int reti = regcomp(&regex, patterns[i], 0);
    reti = regexec(&regex, line, 0, NULL, 0);
    if (!reti) {
      match++;
    }
    regfree(&regex);
  }
  return match;
}

int option_f(Cmd* cmd, int* p_num, char** patterns) {
  cmd->opt_f++;
  char* patt_file_name = optarg;
  FILE* pat;
  if ((pat = fopen(patt_file_name, "r")) != NULL) {
    char* line = malloc(1024 * sizeof(char));
    char* read;
    while ((read = fgets(line, 1024, pat)) != NULL) {
      if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
      }
      strcpy(patterns[*p_num], line);
      (*p_num)++;
    }
    free(line);
    fclose(pat);
    return 0;
  } else {
    fprintf(stderr, "%s", "grep: No such file or directory\n");
    return 2;
  }
}

int option_i(Cmd* cmd, char* line, char** patterns, int p_num) {
  int match = 0;
  if (cmd->opt_i != 0) {
    match += check_low(line, patterns, p_num);
  }
  return match;
}

void option_v(Cmd* cmd, int* match) {
  if (cmd->opt_v != 0) {
    if (*match == 0) {
      (*match)++;
    } else {
      *match = 0;
    }
  }
}

void option_c(Cmd* cmd, int match, int* cnt_fit_lines,
              int* need_to_print_line) {
  if (cmd->opt_c != 0) {
    if (match != 0) {
      (*cnt_fit_lines)++;
      *need_to_print_line = 0;
    }
  }
}

void option_l(Cmd* cmd, int match, int* need_to_print_line,
              int* write_filename) {
  if (cmd->opt_l != 0 && match != 0) {
    *need_to_print_line = 0;
    *write_filename = 1;
  }
}

void option_lc(Cmd* cmd, int* cnt_fit_lines) {
  if (cmd->opt_l != 0) {
    *cnt_fit_lines = (*cnt_fit_lines > 0) ? 1 : 0;
  }
}

void format_print(Cmd* cmd, int match, int* need_to_print_line, int argc,
                  int optind, char* filename, int num_line, char* line) {
  if (match != 0 && *need_to_print_line != 0) {
    if (argc - optind != 1 && cmd->opt_h == 0) {
      printf("%s:", filename);
    }
    if (cmd->opt_n != 0) {
      printf("%d:", num_line);
    }
    printf("%s", line);
    if (line[strlen(line) - 1] != '\n') {
      printf("\n");
    }
    *need_to_print_line = 0;
  }
}
