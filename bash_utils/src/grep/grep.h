#ifndef SRC_GREP_H
#define SRC_GREP_H

typedef struct cmd {
  int opt_e;
  int opt_i;
  int opt_v;
  int opt_c;
  int opt_l;
  int opt_n;
  int opt_h;
  int opt_s;
  int opt_f;
  int opt_o;
  int opt_err;
  int arg_err;

} Cmd;

void init_options(Cmd* cmd);
int check_low(char* line, char** patterns, int p_cnt);
int find_match(char** patterns, char* line, int p_num);
int option_f(Cmd* cmd, int* p_num, char** patterns);
int option_i(Cmd* cmd, char* line, char** patterns, int p_num);
void option_v(Cmd* cmd, int* match);
void option_c(Cmd* cmd, int match, int* cnt_fit_lines, int* need_to_print_line);
void option_l(Cmd* cmd, int match, int* need_to_print_line,
              int* write_filename);
void option_lc(Cmd* cmd, int* cnt_fit_lines);
void format_print(Cmd* cmd, int match, int* need_to_print_line, int argc,
                  int optind, char* filename, int num_line, char* line);

#endif  //   SRC_GREP_H
