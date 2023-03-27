#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "grep.h"

int main(int argc, char *argv[]) {
  Cmd cmd;
  init_options(&cmd);
  if (argc == 1) {
    printf("Usage: grep [OPTION]... PATTERNS [FILE]...\n");
    printf("Try 'grep --help' for more information.\n");
  }
  int p_cnt = 40;
  int p_len = 1024;
  char **patterns = malloc(p_cnt * sizeof(char *));
  for (int i = 0; i < p_cnt; i++) {
    patterns[i] = malloc(p_len * sizeof(char));
  }
  int p_num = 0;
  char *opts = "e:ivclnhsf:o";
  int opt;
  opterr = 0;
  while ((opt = getopt_long(argc, argv, opts, NULL, NULL)) != -1) {
    if (opt == 'e') {
      cmd.opt_e++;
      if (patterns[p_num] != NULL) {
        strcpy(patterns[p_num], optarg);
      }
      p_num++;
    } else if (opt == 'f') {
      option_f(&cmd, &p_num, patterns);
    } else if (opt == 'o') {
      cmd.opt_o++;
    } else if (opt == 'i') {
      cmd.opt_i++;
    } else if (opt == 'v') {
      cmd.opt_v++;
    } else if (opt == 'c') {
      cmd.opt_c++;
    } else if (opt == 'l') {
      cmd.opt_l++;
    } else if (opt == 'n') {
      cmd.opt_n++;
    } else if (opt == 'h') {
      cmd.opt_h++;
    } else if (opt == 's') {
      cmd.opt_s++;
    } else if (opt == '?') {
      cmd.opt_err++;
    } else if (opt == ':') {
      cmd.arg_err++;
    }
  }
  if (cmd.arg_err != 0) {
    fprintf(stderr, "%s", "grep: invalid option\n");
    for (int i = 0; i < p_cnt; i++) {
      free(patterns[i]);
    }
    free(patterns);
    return 2;
  }
  if (cmd.opt_err != 0) {
    fprintf(stderr, "%s", "grep: option requires an argument\n");
    for (int i = 0; i < p_cnt; i++) {
      free(patterns[i]);
    }
    free(patterns);
    return 2;
  }
  if (p_num == 0) {
    strcpy(patterns[p_num], argv[optind]);
    p_num++;
    optind++;
  }
  for (int i = optind; i < argc; i++) {
    char *filename = argv[i];
    FILE *fin;
    int cnt_fit_lines = 0;
    int num_line = 0;
    if ((fin = fopen(filename, "r")) != NULL) {
      char *read;
      char *line = malloc(1024 * sizeof(char));
      int write_filename = 0;
      while ((read = fgets(line, 1024, fin)) != NULL) {
        num_line++;
        int need_to_print_line = 1;
        int match = find_match(patterns, line, p_num);
        match += option_i(&cmd, line, patterns, p_num);
        option_v(&cmd, &match);
        option_c(&cmd, match, &cnt_fit_lines, &need_to_print_line);
        option_l(&cmd, match, &need_to_print_line, &write_filename);
        format_print(&cmd, match, &need_to_print_line, argc, optind, filename,
                     num_line, line);
      }
      if (cmd.opt_c != 0 && argc - optind == 1) {
        option_lc(&cmd, &cnt_fit_lines);
        printf("%d\n", cnt_fit_lines);
      } else if (argc - optind != 1 && cmd.opt_c != 0 && cmd.opt_h == 0) {
        option_lc(&cmd, &cnt_fit_lines);
        printf("%s:%d\n", filename, cnt_fit_lines);
      }
      if (write_filename != 0) {
        printf("%s\n", filename);
      }
      free(line);
      fclose(fin);
    } else {
      if (cmd.opt_s == 0) {
        fprintf(stderr, "%s", "grep: No such file or directory\n");
      }
    }
  }
  for (int i = 0; i < p_cnt; i++) {
    free(patterns[i]);
  }
  free(patterns);
  return 0;
}
