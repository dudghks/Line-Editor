#ifndef CLL_H_
#define CLL_H_

struct cll_n {
	char *s;
	int bsize;
	struct cll_n *next;
	struct cll_n *prev;
};

typedef struct cll {
	struct cll_n *h;
	struct cll_n *t;
	int len;
} cll;

cll cll_create();

void cll_insert(cll *list, int idx, char *data);

char *cll_remove(cll *list, int idx);

struct cll_n *cll_getnode(cll *list, int idx);

void cll_setnode(cll *list, int idx, char *data);

#endif /* CLL_H_ */
