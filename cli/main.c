#include <lege.h>
#include <optparse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

int main(int argc, char *argv[]) {
  if (argc > 0) {
    PROG_NAME = argv[0];
  }
  struct optparse opts;
  optparse_init(&opts, argv);
  parse_global_opts(&opts);
  run_subcommand(&opts);
  return EXIT_SUCCESS;
}
