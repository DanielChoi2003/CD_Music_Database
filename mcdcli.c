#include "mcddb.h"
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#define BUFSZ 1024

void usage(void);

int main(int argc, char **argv)
{
    int ret, op, cnt, i;
    extern char *optarg; // exported variables; other files can access it.
    extern int optind;
    DBT value;
    char **keys, **values;

    if (argc < 2 || argc > 4)
    {
        usage();
    }

    op = getopt(argc, argv, "a:d:f:g:l");

    if (open_db(NULL) == 1)
    {
        puts("Opening database failed");
    }

    switch (op)
    {
    case 'a':
        // optarg points to the first argument and
        // argv points to the second argument
        if (argc == 4 && optarg != NULL && strlen(optarg) > 0 && argv[optind] != NULL)
        {
            ret = add_rec(optarg, argv[optind]);
            if (ret == 1)
            {
                printf("Key %s eixsts\n", optarg);
                exit(EXIT_FAILURE);
            }
            else if (ret < 0)
            {
                perror("mcdcli.c: add_rec");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            usage(); // prints a how-to manual
        }
        break;

    case 'd':
        if (argc == 3 && optarg != NULL)
        {
            ret = del_rec(optarg);
            if (ret == 1)
            {
                printf("Key '%s' not found\n", optarg);
                exit(EXIT_FAILURE);
            }
            else if (ret < 0)
            {
                perror("mcdcli.c: del_rec");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            usage();
        }
        break;
    case 'f': // find a record
        if (argc == 3 && optarg != NULL)
        {
            memset(&value, 0, sizeof(DBT));
            ret = find_rec(optarg, &value);

            if (ret == 1)
            {
                printf("Key '%s' not found\n", optarg);
                exit(EXIT_FAILURE);
            }
            else if (ret < 0)
            {
                perror("mcdcli.c: find_rec");
                exit(EXIT_FAILURE);
            }
            printf("%s|%.*s\n", optarg, (int)value.size, (char *)value.data);
        }
        else
        {
            usage();
        }
        break;

    case 'g': // get a record
        if (argc == 3 && optarg != NULL)
        {
            memset(&value, 0, sizeof(DBT));

            ret = get_rec(optarg, &value);
            if (ret == 1)
            {
                printf("Key '%s' not found\n", optarg);
                exit(EXIT_FAILURE);
            }
            else if (ret < 0)
            {
                perror("mcdcli.c: get_rec");
                exit(EXIT_FAILURE);
            }

            printf("%s|%.*s\n", optarg, (int)value.size, (char *)value.data);
        }
        else
        {
            usage();
        }
        break;

    case 'l':
        if (argc == 2)
        {
            if ((cnt = count_recs()) == 0)
            {
                puts("No records in database");
                exit(EXIT_FAILURE);
            }

            // allocate memory for an array of pointers
            if ((keys = (char **)malloc(sizeof(char *) * cnt)) == NULL)
            {
                puts("mcdcli.c: malloc keys");
                exit(EXIT_FAILURE);
            }

            if ((values = (char **)malloc(sizeof(char *) * cnt)) == NULL)
            {
                puts("mcdcli.c: malloc values");
                exit(EXIT_FAILURE);
            }

            // allocate memory that is pointed by each pointer in the array
            for (i = 0; i < cnt; i++)
            {
                if ((keys[i] = (char *)malloc(BUFSZ)) == NULL)
                {
                    puts("mcdcli.c: malloc keys[i]");
                    exit(EXIT_FAILURE);
                }
                if ((values[i] = (char *)malloc(BUFSZ)) == NULL)
                {
                    puts("mcdcli.h: malloc values[i]");
                    exit(EXIT_FAILURE);
                }
            }
            ret = list_recs(keys, values);
            if (ret == 0)
            {
                perror("mcdcli.c: list_recs");
                exit(EXIT_FAILURE);
            }

            for (i = 0; i < cnt; i++)
            {
                printf("%.*s|%.*s\n", (int)strlen(keys[i]), keys[i], (int)strlen(values[i]), values[i]);
            }
        }
        else
        {
            usage();
        }
        break;

    default:
        usage();

    } // switch

    close_db();
    exit(EXIT_SUCCESS);
}

void usage(void)
{
    puts("USAGE: mcdcli \
    \n\t{-a <key> <value>} \
    \n\t{-d <key>}\
    \n\t{-f <key>}\
    \n\t{-g <key>}\
    \n\t{-l}");
    exit(EXIT_FAILURE);
}