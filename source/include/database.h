#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <syslog.h>

/**
 * Inserts data into the "PAYLOAD" table in the "payload.db" SQLite database.
 * 
 * If the table "PAYLOAD" does not exist, this function will create it with the following schema:
 * - ID: INTEGER PRIMARY KEY, auto-incremented
 * - topics: TEXT, cannot be NULL
 * - DATA: INTEGER, cannot be NULL
 * - TIMESTAMP: DATETIME, defaults to the current timestamp on insertion
 * 
 * The function performs the following operations:
 * 1. Opens or creates the SQLite database "payload.db".
 * 2. Creates the "PAYLOAD" table if it does not already exist.
 * 3. Inserts a row into the "PAYLOAD" table with the given `topics` and `data`.
 * 4. Handles errors by printing messages and cleaning up resources if needed.
 * 5. Closes the database connection before returning.
 *
 * Parameters:
 * - char *topics: A string containing the topic information to insert into the database.
 * - char *data: A string representing the data to be inserted into the database.
 *
 * Returns:
 * - 0 on success.
 * - SQLITE_CANTOPEN if the database cannot be opened.
 * - SQLITE_ERROR if there is an error in executing the SQL statement.
 */
int table_insert(char *topics, char *data);