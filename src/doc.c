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
	return cll_getnode(&d->body, idx)->s;
}

void doc_setline(doc *d, int idx, char *data) {
	cll_setnode(&d->body, idx, data);
}

void doc_insertinline(doc *d, int idx, int l_idx, char *data) {
	struct cll_n *n = cll_getnode(&d->body, idx);
	int dl = strlen(data);
	int nl = strlen(n->s);
	int i;
	if(dl + nl >= n->bsize || l_idx + dl >= n->bsize) {
		n->bsize = nl > l_idx ? dl + nl : l_idx + dl;
		n->s = realloc(n->s, n->bsize * 2 + 1);
	}


	for(i = nl + dl - 1; i >= l_idx + dl; i--) {
		n->s[i] = n->s[i - dl];
	}

	if(l_idx > nl) {
		for(i = nl; i < l_idx; i++) {
			n->s[i] = ' ';
		}
		i += dl;
		n->s[i--] = '\0';
	} else {
		n->s[nl + dl] = '\0';
	}

	for(; i >= l_idx; i--) {
		n->s[i] = data[--dl];
	}
}

void doc_replaceinline(doc *d, int idx, int l_idx, char *data) {
	struct cll_n *n = cll_getnode(&d->body, idx);
	int dl = strlen(data);
	int nl = strlen(n->s);
	int i;

	if(dl + l_idx >= n->bsize) {
		n->s = realloc(n->s, (n->bsize = (dl + l_idx) * 2 + 1));
	}

	for(i = 0; i < dl; i++) {
		n->s[l_idx + i] = data[i];
	}

	for(i = nl; i < l_idx; i++) {
		n->s[i] = ' ';
	}

	if(l_idx + dl > nl) {
		n->s[l_idx + dl] = '\0';
	}
}

void doc_deletefromline(doc *d, int idx, int l_idx, int amt) {
	struct cll_n *n = cll_getnode(&d->body, idx);
	int nl = strlen(n->s);
	int i;

	if(l_idx + amt >= nl) {
		n->s[l_idx] = '\0';
		return;
	}

	for(i = l_idx; i < nl - amt; i++) {
		n->s[i] = n->s[i + amt];
	}
	n->s[i] =  '\0';
}

void doc_rename(doc *d, char *name) {
	int len = strlen(name);
	if(len > d->name_size) {
		d->name = realloc(d->name, len);
	}
	strcpy(d->name, name);
}







