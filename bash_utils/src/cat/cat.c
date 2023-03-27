#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cat_src.h"

int main(int argc, char *argv[]) {
  Options options;
  init_options(&options);
  int k = 1;
  int check_end = 0;
  while (check_end == 0 && k < argc) {
    if (isflag(argv[k])) {
      extract_f(&options, argv[k]);
      k++;
    } else {
      check_end = 1;
    }
  }
  if (options.f_error == 0 && argc != 1) {
    for (int f_n = k; f_n < argc; f_n++) {
      char *file_name = argv[f_n];
      print_sym(file_name, options);
    }
  } else {
    fprintf(stderr, "%s", "cat: illegal option\n");
  }
  return 0;
}
