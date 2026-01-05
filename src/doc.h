#include "cll.h"

#ifndef DOC_H_
#define DOC_H_

typedef struct doc {
	cll body;
	char* name;
	int name_size;
} doc;

doc doc_create();

void doc_insertline(doc *d, int idx, char *data);

char *doc_deleteline(doc *d, int idx);

char *doc_getline(doc *d, int idx);

void doc_setline(doc *d, int idx, char *data);

void doc_rename(doc *d, char *name);

#endif /* DOC_H_ */
