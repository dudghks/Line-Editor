#include <stddef.h>

#ifndef UTILS_H_
#define UTILS_H_

typedef enum {
       	CMD_UNKNOWN = 0,	
	CMD_INSERT,
	CMD_DELETE,
	CMD_INSERTINLINE,
	CMD_REPLACEINLINE,
	CMD_DELETEFROMLINE,
	CMD_PRINT,
	CMD_RENAME,
	CMD_INFO,
	CMD_EOF,
	CMD_SAVEFILE,
	CMD_OPENFILE,
	CMD_NPCOMPILERUNC,
	CMD_HELP,
	CMD_COUNT
} cmd;

int readline(char **b, size_t *size);

int strcmp_ci(char *a, char *b);

cmd identify_command(char *s);

#endif /* UTILS_H_ */
