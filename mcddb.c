#include "mcddb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <db.h>

static DB *db; // handle for the database

int open_db(char *dbpath)
{

    int ret;

    char *default_path = "./mcd.db";

    // if dbpath is NULL, then assume default_path
    if (!dbpath)
    {
        if ((dbpath = malloc(strlen(default_path) + 1)) != NULL)
        {
            strcpy(dbpath, default_path); // dest, source
        }
        else
        {
            return -1;
        }
    }
    if ((ret = db_open(dbpath, DB_BTREE, DB_CREATE, 0666, NULL, NULL, &db)) != 0)
    {
        return ret; // failed
    }

    return 0;
}

int close_db(void)
{
    // close() automatically syncs.
    // any open cursors are also closed.
    db->close(db, 0);
}

int add_rec(char *kbuf, char *vbuf)
{
    int ret;
    DBT key, value;

    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));

    key.data = kbuf;
    key.size = strlen(kbuf);
    value.data = vbuf;
    value.size = strlen(vbuf);

    ret = db->put(db, NULL, &key, &value, DB_NOOVERWRITE);

    if (ret == DB_KEYEXIST)
    {
        return 1;
    }

    // other errors
    if (ret != 0)
    {
        return ret;
    }

    db->sync(db, 0);
    return 0;
}

int del_rec(char *kbuf)
{
    int ret;
    DBT key;

    memset(&key, 0, sizeof(DBT));

    key.data = kbuf;
    key.size = strlen(kbuf);

    ret = db->del(db, NULL, &key, 0);

    if (ret == DB_NOTFOUND)
    {
        return 1;
    }
    else if (ret != 0)
    {
        return ret;
    }

    db->sync(db, 0);
    return 0;
}

int find_rec(char *kbuf, DBT *value)
{

    int ret;
    DBT key;
    DBC *cursor = NULL;

    memset(&key, 0, sizeof(DBT));

    key.data = kbuf;
    key.size = strlen(kbuf);

    ret = db->cursor(db, NULL, &cursor, 0);
    if (ret != 0)
    {
        return ret; // error
    }

    while ((ret = cursor->get(cursor, &key, value, DB_NEXT)) != DB_NOTFOUND)
    {

        /*
         * db does not store null-terminated strings
         * use strncmp to limited the bytes comapred to the length of the search string
         * strcmp (DOESN'T HAVE N) works only on null-terminated strings.
         * strncmp compares only up to n characters (third argument)
         * make sure that the size are the same if they do have the same first n characters
         */
        if (!strncmp(key.data, kbuf, strlen(kbuf)))
        {
            if (key.size == strlen(kbuf))
            {
                break;
            }
        }
    }

    if (ret == DB_NOTFOUND)
    {
        return 1;
    }
    else if (ret != 0)
    {
        return ret;
    }

    return 0;
}

int get_rec(char *kbuf, DBT *value)
{
    int ret;
    DBT key;

    memset(&key, 0, sizeof(DBT));

    key.data = kbuf;
    key.size = strlen(kbuf);

    ret = db->get(db, NULL, &key, value, 0);

    if (ret == DB_NOTFOUND)
    {
        return 1;
    }
    else if (ret != 0)
    {
        return ret;
    }

    return 0;
}

int count_recs(void)
{
    int ret, num = 0;
    DBT key;
    DBC *dbc;

    memset(&key, 0, sizeof(DBT));

    ret = db->cursor(db, NULL, &dbc, 0);
    if (ret != 0)
    {
        return 0;
    }

    while (dbc->get(dbc, &key, NULL, DB_NEXT) == 0)
    {
        num++;
    }

    return num;
}

int list_recs(char **keys, char **values)
{
    int ret, cnt = 0;
    DBT key, value;
    DBC *dbc;

    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));

    ret = db->cursor(db, NULL, &dbc, 0);
    if (ret != 0)
    {
        return ret;
    }

    /*
     * Fun Fact:
     * Ordinarily, when you call db->get(),
     * the memory to which &key and &value point to is valid
     * until the next call to any db function that uses the db handle.
     */
    while ((dbc->get(dbc, &key, &value, DB_NEXT)) == 0)
    {
        memcpy(keys[cnt], key.data, key.size);
        memcpy(values[cnt], value.data, value.size);
        cnt++;
    }

    return cnt;
}