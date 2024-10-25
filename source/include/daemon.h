#pragma once
#ifndef BECOME_DAEMON_H
#define BECOME_DAEMON_H
#define BD_NO_CHDIR          01
#define BD_NO_CLOSE_FILES    02 
#define BD_NO_REOPEN_STD_FDS 04
#define BD_NO_UMASK0        010
#define BD_MAX_CLOSE       8192
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <sys/file.h>
#include <stdio.h>
#include <sys/stat.h>

/**
 * Converts the running process into a daemon.
 * 
 * The function performs several steps to achieve daemonization:
 * 1. Forks the process and exits the parent to ensure the child is a background process.
 * 2. Creates a new session and sets the process group ID to detach from any controlling terminal.
 * 3. Forks a second time to prevent the daemon from reacquiring a terminal.
 * 4. Sets the file mode creation mask to 0 if `BD_NO_UMASK0` is not set.
 * 5. Changes the working directory to the root (`/`) if `BD_NO_CHDIR` is not set.
 * 6. Closes all open file descriptors if `BD_NO_CLOSE_FILES` is not set.
 * 7. Reopens standard file descriptors (stdin, stdout, stderr) to `/dev/null` if `BD_NO_REOPEN_STD_FDS` is not set.
 *
 * Returns:
 * - 0 on successful daemonization, or an error code if any step fails.
 */
int become_daemon();

/**
 * Initializes and starts the daemon process.
 * 
 * This function first calls `become_daemon` to daemonize the process.
 * Then, it sets up system logging for the daemon with the identifier "mqttbd".
 * Logs any errors or information messages related to the daemon’s startup.
 *
 * Returns:
 * - 0 on success, or `EXIT_FAILURE` if daemonization fails.
 */
int daemon_initialize();
#endif