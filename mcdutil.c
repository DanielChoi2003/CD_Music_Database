#include "mcdutil.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSZ 1024

int mcd_getstr(char buf[], int len)
{

    int c, i = 0;

    while ((c = getchar()) != '\n' && i < len)
    {
        buf[i] = c;
        i++;
    }
    buf[i] = '\0';
    return i;
}