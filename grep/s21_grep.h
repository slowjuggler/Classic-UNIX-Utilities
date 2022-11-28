#ifndef S21_GREP_H
#define S21_GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 1024
#define MAX_COUNTER 32

struct flags {
  unsigned char flag_e, flag_i, flag_v, flag_c, flag_l, flag_n, flag_h, flag_s,
      flag_f, flag_o;
};

struct file_params {
  unsigned char equal, flag_template, line_equal, line_number, file_flag,
      flag_line_same, template_print_on, flag_exit, flag_end, flag_dot, result;
};

int result_processing(char *filename, char *str_input, int *template_counter,
                      struct flags *flgp, struct file_params *flp, int counter);
void printing_result_on(char *string_result, char *filename, struct flags *flgp,
                        struct file_params *flp);
void seek_equal(char *template, char *str_input, struct flags *flgp,
                struct file_params *flp, char *filename);
void get_optind(int argc, char **argv, struct flags *flgp,
                char template[MAX_SIZE][MAX_COUNTER]);
void fill_template(struct flags *flgp, char template[MAX_SIZE][MAX_COUNTER],
                   char *string, int *counter);
void flags_parser(int argc, char **argv, struct flags *flgp,
                  char template[MAX_SIZE][MAX_COUNTER], int *template_counter);
void file_processing(int argc, char **argv, struct flags *flgp,
                     char template[MAX_SIZE][MAX_COUNTER],
                     int *template_counter);
void file_result(char **argv, struct flags *flgp, struct file_params *flp);
void is_flag_template(char *filename, char template[MAX_SIZE][MAX_COUNTER],
                      int *template_counter, struct file_params *flp);
void is_file_just_one_func(int argc, char **argv, int *is_file_just_one);
void lines_processing(char **argv, FILE *fp, char *filename, struct flags *flgp,
                      struct file_params *flp,
                      char template[MAX_SIZE][MAX_COUNTER],
                      int *template_counter);
void seek_symbols(char *template, char *temp, char *str_input,
                  struct flags *flgp, struct file_params *flp, int i);
void template_regexpression_parser(char *template, struct file_params *flp);

#endif  //  S21_GREP_H
