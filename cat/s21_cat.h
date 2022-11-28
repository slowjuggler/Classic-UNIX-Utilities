#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 4096

struct flags {
  int flag_b, flag_e, flag_n, flag_s, flag_t, flag_v;
};

void line_number_noempty(char *str_input, int *line);
void line_number(char *str_input, int *line);
void line_end(char *str_input);
void hidden_symbols(char *str_input);
void tabulation(char *str_input);
void param_parser(int argc, char **argv, struct flags *flgp);
void file_processing(int argc, char **argv, struct flags *flgp);

#endif  // S21_CAT_H
