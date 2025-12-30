#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

int readline(char **b, size_t *size) {
        size_t i = 0;
        int c = getchar();
        while(c != '\n' && c != EOF) {
                if(i + 1 >= *size) {
                         *b = realloc(*b, *size *= 2);
                }
                (*b)[i++] = c;
                c = getchar();
        }
        (*b)[i] = '\0';
        return c != EOF || i != 0;
}

