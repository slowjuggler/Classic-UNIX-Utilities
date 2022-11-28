#include "s21_cat.h"

int main(int argc, char **argv) {
  struct flags init_flags = {0};
  param_parser(argc, argv, &init_flags);
  file_processing(argc, argv, &init_flags);
  return 0;
}
