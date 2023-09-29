#include <string.h>

#include "sc_cli.h"
#include "sc_error.h"
#include "sc_gui.h"

int main(int argc, char **argv) {
  int exit_status;

  if (argc == 1) {
    exit_status = smartcalc_gui(argc, argv);
  } else if (strcmp(argv[1], "-t") == 0) {
    exit_status = smartcalc_cli();
  } else if (strcmp(argv[1], "-h") == 0) {
    sc_print_usage();
    exit_status = SC_SUCCESS;
  } else {
    sc_error_program_arg(argv[1]);
    exit_status = SC_FAILURE;
  }

  return (exit_status);
}
