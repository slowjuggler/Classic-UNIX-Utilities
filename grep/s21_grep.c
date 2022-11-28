#include "s21_grep.h"

int main(int argc, char **argv) {
  struct flags init_flags = {0};
  char template[MAX_SIZE][MAX_COUNTER] = {0};
  int template_counter = 1;
  flags_parser(argc, argv, &init_flags, template, &template_counter);
  file_processing(argc, argv, &init_flags, template, &template_counter);
  return 0;
}
