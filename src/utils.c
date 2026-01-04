#include "utils.h"
#include <ctype.h>
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

int strcmp_ci(char *a, char *b) {
        for( ; *a != '\0' && *b != '\0' ; a++, b++) {
                if(tolower(*a) != tolower(*b)) {
                        return 0;
                }
        }
        return *a == *b;
}

cmd identify_command(char *s) {
	switch (*s) {
		case 'D':
		case 'd':
			return strcmp_ci(s, "delete") ? CMD_DELETE : CMD_UNKNOWN;
		case 'e':
		case 'E':
			return strcmp_ci(s, "eof") ? CMD_EOF : CMD_UNKNOWN;
		case 'h':
		case 'H':
			return strcmp_ci(s, "help") ? CMD_HELP : CMD_UNKNOWN;
		case 'I':
		case 'i':
			if(strcmp_ci(s, "insert")) {
				return CMD_INSERT;
			}
			return strcmp_ci(s, "info") ? CMD_INFO : CMD_UNKNOWN;
		case 'n':
		case 'N':
			return strcmp_ci(s, "NP_COMPILERUNC") ? CMD_NPCOMPILERUNC : CMD_UNKNOWN;
		case 'o':
		case 'O':
			return strcmp_ci(s, "openfile") ? CMD_OPENFILE : CMD_UNKNOWN;
		case 'p':
		case 'P':
			return strcmp_ci(s, "print") ? CMD_PRINT : CMD_UNKNOWN;
		case 'r':
		case 'R':
			return strcmp_ci(s, "rename") ? CMD_RENAME : CMD_UNKNOWN;
		case 's':
		case 'S':
			return strcmp_ci(s, "savefile") ? CMD_SAVEFILE : CMD_UNKNOWN;
	}
	return CMD_UNKNOWN;
}

