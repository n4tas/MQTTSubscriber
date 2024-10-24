#pragma once
#ifndef BECOME_DAEMON_H
#define BECOME_DAEMON_H
#define BD_NO_CHDIR          01
#define BD_NO_CLOSE_FILES    02 
#define BD_NO_REOPEN_STD_FDS 04
int become_daemon();
#endif
#define BD_NO_UMASK0        010
#define BD_MAX_CLOSE       8192
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <sys/file.h>
#include <stdio.h>
#include <sys/stat.h>

/**
 * Makes the program a daemon process.
 */
int become_daemon();

/**
 * Daemon on success function which initializes daemon process.
 */
int daemon_initialize();