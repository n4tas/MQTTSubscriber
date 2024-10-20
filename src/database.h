#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <syslog.h>

int table_insert(char *topics, char *data);