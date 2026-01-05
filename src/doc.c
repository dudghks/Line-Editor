#include "doc.h"
#include "cll.h"
#include <stdlib.h>
#include <string.h>

doc doc_create() {
	doc o;
	o.body = cll_create();
	o.name = malloc(16);
	o.name_size = 16;
	strcpy(o.name, "Untitled");
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

void doc_rename(doc *d, char *name) {
	int len = strlen(name);
	if(len > d->name_size) {
		d->name = realloc(d->name, len);
	}
	strcpy(d->name, name);
}
