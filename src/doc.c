#include "doc.h"
#include "cll.h"
#include <stdlib.h>
#include <string.h>

doc doc_create() {
	doc o;
	o.body = cll_create();
	o.name = malloc(16);
	o.name_len = 16;
	o.name[0] = '\0';
	strcat(o.name, "Untitled");
	return o;
}

void doc_insertline(doc *d, int idx, char *data) {
	cll_insert(&d->body, idx, data);
}

char *doc_deleteline(doc *d, int idx) {
	return cll_remove(&d->body, idx);
}

char *doc_getline(doc *d, int idx) {
	return cll_getline(&d->body, idx);
}

void doc_setline(doc *d, int idx, char *data) {
	cll_setline(&d->body, idx, data);
}
