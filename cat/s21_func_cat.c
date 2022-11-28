#define _POSIX_C_SOURCE 200809L
#include "s21_cat.h"

void line_number_noempty(char *str_input, int *line) {
  int len = strlen(str_input);
  char str_output[MAX_SIZE] = {0};
  if (len >= 2) {
    *line += 1;
    sprintf(str_output, "%6d\t", *line);
    strcat(str_output, str_input);
    strcpy(str_input, str_output);
  }
}

void line_number(char *str_input, int *line) {
  *line += 1;
  char str_output[MAX_SIZE] = {0};
  sprintf(str_output, "%6d\t", *line);
  strcat(str_output, str_input);
  strcpy(str_input, str_output);
}

void line_end(char *str_input) {
  int len = strlen(str_input);
  if (str_input[len - 1] == 10) {
    str_input[len - 1] = 36;
    str_input[len] = 10;
    str_input[len + 1] = 0;
  }
  str_input[len + 1] = 0;
}

void hidden_symbols(char *str_input) {
  int len = strlen(str_input);
  if (str_input[len + 1] != 0 && str_input[len - 1] != 10 &&
      str_input[len - 1] != 102) {
    char *temp = str_input + len + 1;
    len = len + strlen(temp) + 1;
  }
  char str_output[MAX_SIZE] = {0};
  int j = 0;
  for (int i = 0; i < len; i++) {
    if (str_input[i] >= 0 && str_input[i] < 32 && str_input[i] != 9 &&
        str_input[i] != 10) {
      str_output[j++] = 94;
      str_output[j++] = str_input[i] + 64;
    } else if (str_input[i] == 127) {
      str_output[j++] = 94;
      str_output[j++] = str_input[i] - 64;
    } else {
      str_output[j++] = str_input[i];
    }
  }
  strcpy(str_input, str_output);
}

void tabulation(char *str_input) {
  char str_output[MAX_SIZE] = {0};
  int len = strlen(str_input);
  int j = 0;
  for (int i = 0; i < len; i++) {
    if (str_input[i] == 9) {
      str_output[j++] = 94;
      str_output[j++] = str_input[i] + 64;
    } else {
      str_output[j++] = str_input[i];
    }
  }
  strcpy(str_input, str_output);
}

void param_parser(int argc, char **argv, struct flags *flgp) {
  while (1) {
    static struct option const long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                                 {"number", 0, 0, 'n'},
                                                 {"squeeze-blank", 0, 0, 's'},
                                                 {0, 0, 0, 0}};
    int option_index = 0;
    int c = 0;
    if ((c = getopt_long(argc, argv, "+benstvET", long_options,
                         &option_index)) == -1) {
      break;
    }
    switch (c) {
      case 'b':
        flgp->flag_b = 1;
        break;
      case 'e':
        flgp->flag_e = 1;
        flgp->flag_v = 1;
        break;
      case 'n':
        flgp->flag_n = 1;
        break;
      case 's':
        flgp->flag_s = 1;
        break;
      case 't':
        flgp->flag_t = 1;
        flgp->flag_v = 1;
        break;
      case 'v':
        flgp->flag_v = 1;
        break;
      case 'E':
        flgp->flag_e = 1;
        break;
      case 'T':
        flgp->flag_t = 1;
        break;
      default:
        fprintf(stderr, "usage: cat [-benstvET] [file ...]\n");
        exit(1);
    }
  }
}

void file_processing(int argc, char **argv, struct flags *flgp) {
  while (optind < argc) {
    FILE *fp = fopen(argv[optind++], "r");
    if (fp != NULL) {
      int line = 0;
      int prev_line_empty = 0;
      int current_line_empty = 0;
      while (!feof(fp)) {
        int zero = 0;
        char *str_input = NULL;
        size_t str_size = MAX_SIZE;
        str_input = (char *)malloc(str_size * sizeof(char));
        if (str_input != NULL) {
          int c = 0;
          if ((c = getline(&str_input, &str_size, fp)) >= 1) {
            int length = strlen(str_input);
            if (length <= 2) {
              current_line_empty = 1;
            } else {
              current_line_empty = 0;
            }
            if (flgp->flag_s) {
              if (current_line_empty && prev_line_empty) {
                zero = 1;
                if (flgp->flag_n && !(flgp->flag_b)) {
                  line--;
                }
              }
            }
            prev_line_empty = current_line_empty;
            if (flgp->flag_t) {
              tabulation(str_input);
            }
            if (flgp->flag_b) {
              line_number_noempty(str_input, &line);
            }
            if (flgp->flag_e) {
              line_end(str_input);
            }
            if (flgp->flag_v) {
              hidden_symbols(str_input);
            }
            if (flgp->flag_n && !(flgp->flag_b)) {
              line_number(str_input, &line);
            }
            if (zero == 0) {
              printf("%s", str_input);
            }
          }
          free(str_input);
        } else {
          perror("Can't allocate memory\n");
          fclose(fp);
          exit(1);
        }
      }
      fclose(fp);
    } else {
      perror(argv[optind - 1]);
      exit(1);
    }
  }
}
