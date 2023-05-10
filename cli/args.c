#include <lege.h>
#include <optparse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

const char *PROG_NAME = "lege";

static const struct optparse_long GLOBAL_OPTS[] = {
    {.longname = "help", .shortname = 'h', .argtype = OPTPARSE_NONE},
    {.longname = "version", .shortname = 'v', .argtype = OPTPARSE_NONE},
    {0},
};

static const char *const HELP_TEXT =
    "Usage: %s [global_options...] subcommand[subcommand_options...]\n\n"
    "Command line interface to the Lower Elements Game Engine\n\n"
    "Global Options:\n"
    "  --help                    Print this help and exit\n"
    "  --version                 Print program version and exit\n\n"
    "Subcommands:\n"
    "  help                      Print this help and exit\n"
    "  run                       Run a LEGE application without compiling it\n"
    "  -version                  Print program version and exit\n\n"
    "Report bugs to Michael Connor Buchan <mikey@blindcomputing.org>\n";

void parse_global_opts(struct optparse *opts) {
  int c;
  while (1) {
    c = optparse_long(opts, GLOBAL_OPTS, NULL);
    switch (c) {
    case -1:
      return;
    case 'v':
      puts("lege " LEGE_VERSION_STRING);
      exit(EXIT_SUCCESS);
    case 'h':
      printf(HELP_TEXT, PROG_NAME);
      exit(EXIT_SUCCESS);
    default:
      fprintf(stderr, "%s: Error: %s\n", PROG_NAME, opts->errmsg);
      fprintf(stderr, HELP_TEXT, PROG_NAME);
      exit(EXIT_FAILURE);
    }
  }
}

void cmd_help(struct optparse *opts) {
  (void)opts;
  printf(HELP_TEXT, PROG_NAME);
}

void cmd_version(struct optparse *opts) {
  (void)opts;
  puts("lege " LEGE_VERSION_STRING);
}

static const struct subcommand {
  const char *name;
  void (*run)(struct optparse *);
} SUBCOMMANDS[] = {
    {.name = "help", .run = cmd_help},
    {.name = "run", .run = cmd_run},
    {.name = "version", .run = cmd_version},
    {0},
};

void run_subcommand(struct optparse *opts) {
  const char *subcommand = optparse_arg(opts);
  if (!subcommand) {
    printf(HELP_TEXT, PROG_NAME);
  } else
    for (const struct subcommand *sc = SUBCOMMANDS; sc->name; ++sc) {
      if (strcmp(subcommand, sc->name) == 0) {
        sc->run(opts);
        return;
      }
    }
  fprintf(stderr, "%s: Error: Unknown subcommand -- '%s'\n", PROG_NAME,
          subcommand);
  printf(HELP_TEXT, PROG_NAME);
}
