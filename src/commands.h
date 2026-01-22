#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "doc.h"

int cmd_insert(doc *document, char *l);

int cmd_delete(doc *document, char *l);

int cmd_insertinline(doc *document, char *l);

int cmd_replaceinline(doc *document, char *l);

int cmd_deletefromline(doc *document, char *l);

int cmd_print(doc *document, char *l);

int cmd_rename(doc *document, char *l);

int cmd_info(doc *document, char *l);

int cmd_eof(doc *document, char *l);

int cmd_savefile(doc *document, char *l);

int cmd_openfile(doc *document, char *l);

int cmd_npcompilerunc(doc *document, char *l);

int cmd_help(doc *document, char *l);

int cmd_unknown(doc *document, char *l);

#endif /* COMMANDS_H_ */
