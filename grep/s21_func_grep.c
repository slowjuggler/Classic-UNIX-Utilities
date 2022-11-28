#include "s21_grep.h"

void printing_result_on(char *string_result, char *filename, struct flags *flgp,
                        struct file_params *flp) {
  if (!flgp->flag_h && !flgp->flag_l && !flgp->flag_c && flp->file_flag &&
      !flp->flag_line_same) {
    printf("%s:", filename);
  }
  if (!flgp->flag_l) {
    if (flgp->flag_n && !flgp->flag_c && !flp->flag_line_same) {
      printf("%d:", flp->line_number);
    }
    if (!flgp->flag_c) {
      int str = strlen(string_result);
      if (string_result[str - 1] != 10) {
        printf("%s\n", string_result);
      } else {
        printf("%s", string_result);
      }
    }
  }
}

void seek_symbols(char *template, char *temp, char *str_input,
                  struct flags *flgp, struct file_params *flp, int i) {
  int len_template = strlen(template);
  int k = 0;
  flp->result = 0;
  if (flp->flag_end) {
    i--;
  }
  if (flp->flag_dot && str_input[i] >= 32) {
    flp->result = 1;
    temp[0] = str_input[i];
    return;
  }
  if (!flgp->flag_i && str_input[i] == template[0]) {
    flp->result = 1;
    k = i;
    for (int j = 0; j < len_template; j++) {
      temp[j] = str_input[k];
      if (str_input[k] != template[j]) {
        flp->result = 0;
        break;
      }
      k++;
    }
  }
  if (flgp->flag_i &&
      (str_input[i] == template[0] || (str_input[i] + 32) == template[0] ||
       (str_input[i] - 32) == template[0])) {
    flp->result = 1;
    k = i;
    for (int j = 0; j < len_template; j++) {
      temp[j] = str_input[k];
      if ((str_input[i] >= 65 && str_input[i] <= 90) ||
          (str_input[i] >= 97 && str_input[i] <= 122)) {
        if (str_input[k] != template[j] && (str_input[k] + 32) != template[j] &&
            (str_input[k] - 32) != template[j]) {
          flp->result = 0;
          break;
        }
      } else if (str_input[k] != template[j]) {
        flp->result = 0;
        break;
      }
      k++;
    }
  }
}

void seek_equal(char *template, char *str_input, struct flags *flgp,
                struct file_params *flp, char *filename) {
  char temp[MAX_SIZE] = {0};
  int len_str_input = strlen(str_input);
  int len_template = strlen(template);
  int delta = len_str_input - len_template;
  flp->flag_line_same = 0;
  if (len_str_input && delta >= 0) {
    for (int i = 0; i < len_str_input; i++) {
      if (flp->flag_end) {
        i = delta;
      }
      seek_symbols(template, temp, str_input, flgp, flp, i);
      if (flp->result) {
        flp->equal = 1;
        if (!flp->flag_line_same) {
          flp->line_equal += 1;
        }
        if (flgp->flag_l || flgp->flag_c) {
          break;
        }
        if (flgp->flag_o && !flgp->flag_v) {
          temp[len_template] = 10;
          temp[len_template + 1] = 0;
          printing_result_on(temp, filename, flgp, flp);
          flp->flag_line_same = 1;
          if (flp->flag_exit || flp->flag_end) {
            break;
          } else {
            continue;
          }
        }
        break;
      }
      if (flp->flag_exit || flp->flag_end) {
        break;
      }
    }
  }
}

void get_optind(int argc, char **argv, struct flags *flgp,
                char template[MAX_SIZE][MAX_COUNTER]) {
  int counter = 1;
  char parameter[MAX_SIZE] = {0};
  while (counter < argc) {
    strcpy(parameter, argv[counter]);
    if (parameter[0] != 45) {
      if (!flgp->flag_f && !flgp->flag_e) {
        strcpy(template[0], parameter);
      }
      break;
    }
    counter++;
  }
}

void fill_template(struct flags *flgp, char template[MAX_SIZE][MAX_COUNTER],
                   char *string, int *counter) {
  FILE *ftemp = NULL;
  if ((ftemp = fopen(string, "r")) != NULL) {
    while (!feof(ftemp)) {
      char string[MAX_SIZE] = {0};
      fgets(string, sizeof(template[*counter]), ftemp);
      int len = strlen(string);
      if (len > 1 && string[len - 1] == 10) {
        string[len - 1] = 0;
      }
      strcpy(template[(*counter)++], string);
      if (*counter >= MAX_COUNTER) {
        fprintf(stderr, "Too many templates!\n");
        fclose(ftemp);
        exit(1);
      }
    }
    fclose(ftemp);
  } else if (!flgp->flag_s) {
    perror(string);
  }
}

void flags_parser(int argc, char **argv, struct flags *flgp,
                  char template[MAX_SIZE][MAX_COUNTER], int *template_counter) {
  while (optind < argc) {
    int c = 0;
    while ((c = getopt(argc, argv, "ce:hif:lnosv")) != -1) {
      switch (c) {
        case 'f':
          flgp->flag_f = 1;
          fill_template(flgp, template, optarg, template_counter);
          break;
        case 'e':
          flgp->flag_e = 1;
          if (!optarg) {
            fprintf(stderr,
                    "usage: s21_grep [-chilnosv] [-e pattern] [-f file] "
                    "[pattern] [file ...]\n");
            exit(1);
          }
          strcpy(template[(*template_counter)++], optarg);
          break;
        case 'i':
          flgp->flag_i = 1;
          break;
        case 'v':
          flgp->flag_v = 1;
          break;
        case 'c':
          flgp->flag_c = 1;
          break;
        case 'l':
          flgp->flag_l = 1;
          break;
        case 'n':
          flgp->flag_n = 1;
          break;
        case 'h':
          flgp->flag_h = 1;
          break;
        case 's':
          flgp->flag_s = 1;
          break;
        case 'o':
          flgp->flag_o = 1;
          break;
        default:
          fprintf(stderr,
                  "usage: s21_grep [-chilnosv] [-e pattern] [-f file] "
                  "[pattern] [file ...]\n");
          exit(1);
      }
    }
    optind++;
  }
  get_optind(argc, argv, flgp, template);
  optind = 1;
}

void template_regexpression_parser(char *template, struct file_params *flp) {
  int len = strlen(template);
  if (template[0] == 94) {
    flp->flag_exit = 1;
    char temp[MAX_SIZE] = {0};
    int len = strlen(template);
    int j = 0;
    for (int i = 1; i < len; i++) {
      temp[j++] = template[i];
    }
    strcpy(template, temp);
  }
  len = strlen(template);
  if (template[len - 1] == 36) {
    flp->flag_end = 1;
    char temp[MAX_SIZE] = {0};
    int j = 0;
    for (int i = 0; i < len - 1; i++) {
      temp[j++] = template[i];
    }
    strcpy(template, temp);
  }
  if (template[len - 1] == 46) {
    flp->flag_dot = 1;
  }
}

void file_result(char **argv, struct flags *flgp, struct file_params *flp) {
  if (flgp->flag_c && flp->file_flag && !flgp->flag_h) {
    printf("%s:", argv[optind - 1]);
  }
  if (flgp->flag_c && !flgp->flag_v) {
    printf("%d\n", flp->line_equal);
  }
  if (flgp->flag_c && flgp->flag_v && flgp->flag_l) {
    printf("%d\n", flgp->flag_l);
  }
  if (flgp->flag_c && flgp->flag_v && !flgp->flag_l) {
    printf("%d\n", flp->line_number - flp->line_equal);
  }
  if (flgp->flag_l && !flgp->flag_v && flp->equal) {
    printf("%s\n", argv[optind - 1]);
  }
  if (flgp->flag_l && flgp->flag_v) {
    printf("%s\n", argv[optind - 1]);
  }
}

void is_flag_template(char *filename, char template[MAX_SIZE][MAX_COUNTER],
                      int *template_counter, struct file_params *flp) {
  for (int i = 0; i < *template_counter; i++) {
    if (!strcmp(template[i], filename)) {
      flp->flag_template = 1;
      break;
    }
  }
}

void is_file_just_one_func(int argc, char **argv, int *is_file_only_one) {
  char temp[MAX_SIZE] = {0};
  if (optind < argc) {
    strcpy(temp, argv[optind]);
    if (temp[0] != 45) {
      *is_file_only_one = 1;
    }
  }
}

int result_processing(char *filename, char *str_input, int *template_counter,
                      struct flags *flgp, struct file_params *flp,
                      int counter) {
  int ret = 0;
  char temp[MAX_SIZE] = {0};
  strcpy(temp, str_input);
  int len = strlen(str_input);
  temp[len] = 0;
  if (!flp->result) {
    if (flgp->flag_v && counter == *template_counter - 1 && !flgp->flag_c) {
      printing_result_on(temp, filename, flgp, flp);
    }
  }
  if (flp->result) {
    if (!flgp->flag_o && !flgp->flag_v) {
      printing_result_on(temp, filename, flgp, flp);
    }
    ret = 1;
  }
  return ret;
}

void lines_processing(char **argv, FILE *fp, char *filename, struct flags *flgp,
                      struct file_params *flp,
                      char template[MAX_SIZE][MAX_COUNTER],
                      int *template_counter) {
  char temp[MAX_SIZE] = {0};
  while (!feof(fp)) {
    char str_input[MAX_SIZE] = {0};
    if (fgets(str_input, MAX_SIZE, fp) != NULL) {
      flp->line_number += 1;
      flp->template_print_on = 0;
      for (int i = 0; i < *template_counter; i++) {
        flp->result = 0;
        if (strlen(template[i])) {
          strcpy(temp, template[i]);
          flp->flag_exit = 0;
          flp->flag_end = 0;
          template_regexpression_parser(temp, flp);
          seek_equal(temp, str_input, flgp, flp, argv[optind - 1]);
          if (result_processing(filename, str_input, template_counter, flgp,
                                flp, i)) {
            break;
          }
        }
      }
      if (flp->equal && flgp->flag_l) {
        break;
      }
    }
  }
}

void file_processing(int argc, char **argv, struct flags *flgp,
                     char template[MAX_SIZE][MAX_COUNTER],
                     int *template_counter) {
  FILE *fp = NULL;
  int is_file_just_one = 0;
  while (optind < argc) {
    struct file_params init_file = {0};
    char filename[MAX_SIZE] = {0};
    strcpy(filename, argv[optind++]);
    is_flag_template(filename, template, template_counter, &init_file);
    if (filename[0] != 45 && !init_file.flag_template &&
        strcmp(argv[optind - 2], "-f")) {
      is_file_just_one_func(argc, argv, &is_file_just_one);
      init_file.file_flag = is_file_just_one;
      fp = fopen(filename, "r");
      if (fp != NULL) {
        lines_processing(argv, fp, filename, flgp, &init_file, template,
                         template_counter);
        file_result(argv, flgp, &init_file);
        fclose(fp);
      } else if (!flgp->flag_s) {
        perror(argv[optind - 1]);
      }
    }
  }
}
