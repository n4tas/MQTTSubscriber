#pragma once
#include <argp.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include <stdint.h>
#include <stdlib.h>

// Structure to hold command-line arguments.
struct arguments {
    char *host;
    int port;
    int daemon;
};

extern struct arguments arguments;

// Array of options for argument parsing, specifying each possible command-line option.
__attribute__((unused)) static struct argp_option options[] = {
    {"host", 'h', "HOST", 0, "Host name or IP address"},
    {"port", 'p', "PORT", 0, "Port number"},
    {"daemon", 'd', "DAEMON", OPTION_ARG_OPTIONAL, "Daemon"},
    {0}
};

// Function prototype for 'parse_opt', which processes each option.
error_t parse_opt(int key, char *arg, struct argp_state *state);

// Function prototype for 'load_arguments', which initializes and processes arguments from command line.
void load_arguments(int argc, char **argv);