#include "cll.h"
#include <stdlib.h>
#include <string.h>

cll cll_create() {
	cll o;
	o.h = NULL;
	o.t = NULL;
	o.len = 0;
	return o;
}

void cll_insert(cll *list, int idx, char *data) {
	struct cll_n *node = malloc(sizeof(struct cll_n));
	int size = (strlen(data) + 1) * 2;
	char *s = malloc(size);
	struct cll_n *ip;
	strcpy(s, data);
	node->s = s;
	
	if(idx == 0) {
		node->prev = NULL;
		node->next = list->h;
		if(node->next != NULL) {
			list->h->prev = node;
		} else {
			list->t = node;
		}
		list->h = node;
	} else if(idx == list->len || idx == -1) {
                node->next = NULL;
                node->prev = list->t;
		if(node->prev != NULL) {
                	list->t->next = node;
		} else {
			list->h = node;
		}
		list->t = node;
        } else {
		ip = list->h;
		for( ; idx > 0; idx--) {
			ip = ip->next;
		}
		ip->prev->next = node;
		node->prev = ip->prev;

		ip->prev = node;
		node->next = ip;
	}
	list->len++;
}

char *cll_remove(cll *list, int idx) {
	char *o;
	struct cll_n *rnode;
	if(idx == -1 || idx == list->len - 1) {
		rnode = list->t;
		o = rnode->s;
		list->t = rnode->prev;
		if(list->t != NULL) {
			list->t->next = NULL;
		} else {
			list->h = NULL;
		}
	} else if(idx == 0) {
		rnode = list->h;
		o = rnode->s;
		list->h = list->h->next;
		if(list->h != NULL) {
			list->h->prev = NULL;
		} else {
			list->t = NULL;
		}
	} else {
		rnode = list->h;
		for( ; idx > 0; idx--) {
			rnode = rnode->next;
		}
		o = rnode->s;
		rnode->prev->next = rnode->next;
		rnode->next->prev = rnode->prev;
	}
	free(rnode);
	list->len--;
	return o;
}

char *cll_getline(cll *list, int idx) {
	struct cll_n *node = list->h;
	for( ; idx > 0; idx--) {
		node = node->next;
	}
	return node->s;
}

void cll_setline(cll *list, int idx, char *data) {
	struct cll_n *node = list->h;
	int l;
	for( ; idx > 0; idx--) {
		node = node->next;
	}
	l = strlen(data) + 1;
	if(node->bsize < l) {
		node->s = realloc(node->s, node->bsize = l * 2);
	}
	strcpy(node->s, data);
}

