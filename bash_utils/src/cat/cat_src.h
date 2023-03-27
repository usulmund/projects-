#ifndef SRC_CAT_CAT_SRC_H_
#define SRC_CAT_CAT_SRC_H_

typedef struct options {
  int b_f;
  int e_f;
  int n_f;
  int s_f;
  int t_f;
  int v_f;
  int f_error;
} Options;

int isflag(char *str);
void extract_f(Options *options, char *str);
void init_options(Options *options);
void simple_option(Options *options, char *flags);
void flag_t(unsigned char sym, int *stabil);
void flag_e(unsigned char sym);
void flag_v(unsigned char sym, int *stabil);
void flag_n(unsigned char sym, int *line);
void flag_b(unsigned char past, unsigned char cur, int *line);
void flag_s(unsigned char past, unsigned char cur, int *empty_line,
            int *stabil);
void flag_se(unsigned char past, unsigned char cur, int *empty_line,
             int *stabil);
void flag_sn(unsigned char past, unsigned char cur, int *empty_line,
             int *stabil, int *line);
void print_sym(char *file_name, Options options);
#endif  //  SRC_CAT_CAT_SRC_H_
