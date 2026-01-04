#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "doc.h"

int cmd_insert(doc *document, char *s);

int cmd_delete(doc *document, char *s);

int cmd_print(doc *document, char *s);

int cmd_rename(doc *document, char *s);

int cmd_info(doc *document, char *s);

int cmd_eof(doc *document, char *s);

int cmd_savefile(doc *document, char *s);

int cmd_openfile(doc *document, char *s);

int cmd_npcompilerunc(doc *document, char *s);

int cmd_help(doc *document, char *s);

int cmd_unknown(doc *document, char *s);

#endif /* COMMANDS_H_ */
