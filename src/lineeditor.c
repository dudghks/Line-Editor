#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "cll.h"
#include "doc.h"
#include "utils.h"
#include "commands.h"

int main(int argc, char **argv) {
	doc document = doc_create();
	char *b = malloc(16), *p, *tok;
	int i, k = 1, t1, t2;
	size_t bsize = 16, blen;
	cmd command;

	int (*(commands)[CMD_COUNT])() = {
		cmd_unknown,
		cmd_insert,
		cmd_delete,
		cmd_insertinline,
		cmd_replaceinline,
		cmd_deletefromline,
		cmd_print,
		cmd_rename,
		cmd_info,
		cmd_eof,
		cmd_savefile,
		cmd_openfile,
		cmd_npcompilerunc,
		cmd_help	
	};

	if(argc > 1) {
		cmd_openfile(&document, *++argv);
	}

	while(k) {
		printf("> ");
		k = readline(&b, &bsize);

		blen = strlen(b);
		tok = strtok(b, " ");
		
		if(!tok) {
			continue;
		}
		
		command = identify_command(tok);

		if((tok - b) + strlen(tok) == blen) {
			p = NULL;
		} else {
			p = tok + strlen(tok) + 1;
			while(*p == ' ') {
				p++;
			}
		}
	        
		commands[command](&document, p);

		if(command == CMD_EOF) {
			break;
		}
	}
	t1 = document.body.len;
	t2 = 1;
	while((t1 /= 10) > 0) {
		t2++;
	}
	printf("\n----------Document Contents----------\n");
	for(i = 0; i < document.body.len; i++) {
		printf("%*d |%s\n", t2, i + 1, doc_getline(&document, i));
	}
	return 0;
}

