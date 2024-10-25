#include "../include/database.h"

sqlite3 *db = NULL;

int table_insert(char *topics, char *data)
{  
   char *zErrMsg = 0;
   if (sqlite3_open("payload.db", &db) != SQLITE_OK) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return SQLITE_CANTOPEN;
   }
    char *query = sqlite3_mprintf(
        "CREATE TABLE IF NOT EXISTS PAYLOAD("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "topics TEXT NOT NULL, "
        "DATA INTEGER NOT NULL, "
        "TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP ); "
        "INSERT INTO PAYLOAD (topics, DATA) "
        "SELECT '%q', '%q' ",
        topics, data
    );
   if (sqlite3_exec(db, query, NULL, 0, &zErrMsg) != SQLITE_OK) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      sqlite3_close(db);
      return SQLITE_ERROR;
   }
   sqlite3_free(query);
   sqlite3_close(db);
   return 0;
}

