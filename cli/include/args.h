#ifndef LEGE_CLI_ARGS_H
#define LEGE_CLI_ARGS_H

extern const char *PROG_NAME;

void parse_global_opts(struct optparse *opts);
void run_subcommand(struct optparse *opts);

void cmd_run(struct optparse *opts);

#endif
