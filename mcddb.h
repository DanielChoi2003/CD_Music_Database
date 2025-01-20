#ifndef MCDDB_H_ // to prevent redefinition of mcddh.h header file
#define MCDDB_H_

#include <db.h>

/*
 * opens a database in dbpath. if dbpath is NULL, then
 * it assumes path of "./mcd.db".
 * returns 0 on success or -1 on failure.
 */
int open_db(char *dbpath);

/*
 * Sync and close the open database.
 * Always returns 0.
 */
int close_db(void);

/*
 * Add the record with a key of "kbuf" and a value of "vbuf".
 * Returns zero on success, 1 if the key already exists,
 * and errno otherwise (other numbers).
 */
int add_rec(char *kbuf, char *vbuf);

/*
 * delete the record that matches the key "kbuf".
 * On success, it syncs the database to disc and returns 0;
 * On failure, it returns 1 if a key "kbuf" was not found or errno
 * otherwise
 */
int del_rec(char *kbuf);

/*
 * Iterates the database and finds the record that matches the key "kbuf".
 * On success, it returns 0 and store the data into "value".
 * On failure, it reutnrs DB_NOTFOUND or errno for other errors.
 */
int find_rec(char *kbuf, DBT *value);

/*
 * retrieve the record matching the key from the database.
 * Returns 0 on success and stores value in "value".
 * Returns 1 if the key is not found.
 * errno for other errors.
 */
int get_rec(char *kbuf, DBT *value);

/*
 * Count the number of records in the database by iterating
 * through it with a cursor.
 * Returns the number of records on success.
 * Returns 0 if the database is empty or an error occurs.
 */
int count_recs(void);

/*
 * Iterates through the entire database, listing each record with keys.
 * Returns 0 if no records are available.
 * Returns errno on failure.
 */
int list_recs(char **keys, char **values);

#endif