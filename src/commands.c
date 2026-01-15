#include "commands.h"
#include "doc.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


int get_int_arg(char **s, int *o) {
	char *p = *s;
	int olen, slen = strlen(*s);

	while(*p == ' ') {
		p++;
	}
	while(isdigit(*p)) {
		p++;
	}
	if(*p == ' ' || *p == '\0') {
		*p = '\0';
		*o = atoi(*s);
		olen = p - *s;
		*s = ++p;
		return slen - olen; /* num of chars. after arg */
	}
	return -1;
}

int cmd_insert(doc *document, char *l) {
	int arg, rc;
	
	if(!l) {
		doc_insertline(document, -1, "");
		return 1;
	}

	if((rc = get_int_arg(&l, &arg)) == -1) {
		doc_insertline(document, -1, l);
		return 1;
	}
	while(arg - 1 > document->body.len) {
		doc_insertline(document, -1, "");
	}

	if(rc == 0) {
		doc_insertline(document, arg - 1, "");
	} else {
		doc_insertline(document, arg - 1, l);
	}
	return 1;
}

int cmd_delete(doc *document, char *l) {
	int arg1, arg2, rc;

	if(!l) {
		printf("  Missing required line number.\n");
		return -1;
	}

	if((rc = get_int_arg(&l, &arg1)) == -1) {
		printf("  First argument is not a valid number.\n");
		return -1;	
	} else if(rc == 0) {
		arg2 = 1;
	} else {
		rc = get_int_arg(&l, &arg2);
	}

	if(rc == -1) {
		printf("  Second argument is not a valid number.\n");
		return -1;
	}

	if(arg1 < 1 || arg1 > document->body.len) {
		printf("  Line number is out of bounds.\n");
	}
	while(arg2-- > 0 && document->body.len >= arg1) {
		free(doc_deleteline(document, arg1 - 1));
	}
	return 1;
}

int cmd_insertinline(doc *document, char *l) {
	int arg1, arg2, rc;

	if(!l) {
		printf("  Missing line number, line index, and text arguments.\n");
		return -1;
	}

	if((rc = get_int_arg(&l, &arg1)) == -1) {
		printf("  First argument must be a number.\n");
		return -1;
	} else if(rc == 0) {
		printf("  Missing line index and text arguments\n");
		return -1;
	}

	if((rc = get_int_arg(&l, &arg2)) == -1) {
		printf("  Second argument must be a number.\n");
		return -1;
	} else if(rc == 0) {
		printf("  Missing text to be inserted.\n");
		return -1;
	}

	if(arg1 < 1 || arg1 > document->body.len) {
		printf("  Line number is out of bounds.\n");
		return -1;
	}
	if(arg2 < 1) {
		printf("  Starting index must be positive.\n");
		return -1;
	}

	doc_insertinline(document, arg1 - 1, arg2 - 1, l);
	return 1;
}

int cmd_replaceinline(doc *document, char *l) {
	int arg1, arg2, rc;

	if(!l) {
		printf("  Missing line number, line index, and text arguments.\n");
		return -1;
	}

	if((rc = get_int_arg(&l, &arg1)) == -1) {
		printf("  First argument must be a number.\n");
		return -1;
	} else if(rc == 0) {
		printf("  Missing line index and text arguments.\n");
		return -1;
	}

	if((rc = get_int_arg(&l, &arg2)) == -1) {
		printf("  Second argument must be a number.\n");
		return -1;
	} else if(rc == 0) {
		printf("  Missing text to be inserted.\n");
		return -1;
	}

	if(arg1 < 1 || arg1 > document->body.len) {
		printf("  Line number is out of bounds.\n");
		return -1;
	}
	if(arg2 < 1) {
		printf("  Starting index must be positive.\n");
		return -1;
	}
	
	doc_replaceinline(document, arg1 - 1, arg2 - 1, l);
	
	return 1;
}

int cmd_deletefromline(doc *document, char *l) {
	int arg1, arg2, arg3, rc;

	if(!l) {
		printf("  Missing line number, line index, and number of chars to delete\n");
		return -1;
	}

	if((rc = get_int_arg(&l, &arg1)) == -1) {
		printf("  First argument must be a number.\n");
		return -1;
	} else if(rc == 0) {
		printf("  Missing line index and number of chars to delete.\n");
		return -1;
	}

	if((rc = get_int_arg(&l, &arg2)) == -1) {
		printf("  Second argument must be a number.\n");
		return -1;
	} else if(rc == 0) {
		printf("  Missing number of chars to delete.\n");
		return -1;
	}

	if((rc = get_int_arg(&l, &arg3)) == -1) {
		printf("  Third argument must be a number.\n");
		return -1;
	}

	if(arg1 < 1 || arg1 > document->body.len) {
		printf("  Line number is out of bounds.\n");
		return -1;
	}
	if(arg2 < 1) {
		printf("  Starting index must be positive.\n");
		return -1;
	}

	doc_deletefromline(document, arg1 - 1, arg2 - 1, arg3);
	return 1;
}

int cmd_print(doc *document, char *l) {
	int arg1 = 1, arg2 = document->body.len, rc, lmax, e;
	
	if(!l) {
	} else if((rc = get_int_arg(&l, &arg1)) == -1) {
		printf("  First argument is not a valid number.\n");
		return -1;
	} else if(rc > 0) {
		rc = get_int_arg(&l, &arg2);
		if(rc == -1) {
			printf("  Second argument is not a valid number.\n");
		}
	}

	if(arg1 > arg2) {
		printf("  Start line number must be less than or equal to end line number.\n");
		return -1;
	} else if(arg1 > document->body.len) {
		printf("  Start line number exceeds document line count.\n");
		return -1;
	} else if(arg1 <= 0) {
		printf("  Start line number must be a positive number.\n");
		return -1;
	} else if(arg2 > document->body.len) {
		printf("  End line number exceeds document line count.\n");
		return -1;
	}
	
	lmax = arg2;
	e = 1;
	while((lmax /= 10) > 0) {
		e++;
	}

	for(lmax = arg1 - 1; lmax < arg2; lmax++) {
		printf("%*d |%s\n", e, lmax + 1, doc_getline(document, lmax));
	}
	return 1;
}

int cmd_rename(doc *document, char *l) {
	if(!l) {
		printf("  Name not provided.\n");
		return -1;
	}
	doc_rename(document, l);
	return 1;
}

int cmd_info(doc *document, char *l) {
	l++;
	printf("  Title: %s\n", document->name);
	printf("  Line count: %d\n", document->body.len);	
	return 1;
}

int cmd_eof(doc *document, char *l) {
	document++; l++;
	return 1;	
}

int cmd_savefile(doc *document, char *l) {
	int i = 0;
	FILE *fp;

	if(!l) {
		l = document->name;
	}

	fp = fopen(l, "wx");
	if(!fp) {
		printf("  File %s already exists.\n", l);
		return -1;
	}

	for(i = 0; i < document->body.len; i++) {
		fputs(doc_getline(document, i), fp);
		fputc('\n', fp);
	}
	fclose(fp);
	printf("  Document saved as %s\n", l);
	return 1;
}

int cmd_openfile(doc *document, char *l) {
	FILE *fp;
	char *b;
	int fc, bs, bl;

	if(!l) {
		printf("  Missing file name.\n");
		return -1;
	}

	fp = fopen(l, "r");

	if(!fp) {
		printf("  Unable to open file %s (either missing permissions or does not exist)\n", l);
		return -1;
	}

	b = malloc(16);
	bs = 16;
	bl = 0;

	while(document->body.len > 0) {
		free(doc_deleteline(document, 0));
	}

	while((fc = fgetc(fp)) != EOF) {
		if(fc == '\n') {
			b[bl] = '\0';
			doc_insertline(document, -1, b);
			bl = 0;
			continue;
		}
		if(bl + 1 >= bs) {
			b = realloc(b, bs *= 2);
		}
		b[bl++] = fc;
	}

	if(bl != 0) {
		b[bl] = '\0';
		doc_insertline(document, -1, b);
	}
	free(b);
	fclose(fp);
	doc_rename(document, l);
	printf("Successfully opened file %s\n", l);
	return 1;
}

int cmd_npcompilerunc(doc *document, char *l) {
	time_t t = time(NULL);
	char *p = malloc(50), *p2 = malloc(50);
	pid_t pid;
	int i;
	FILE *fp;

	l++;

	sprintf(p, "temp%ld.c", (long)t);

	fp = fopen(p, "wx");
	
	if(!fp) {
		printf("  Unable to create temp file.\n");
		return -1;
	}
	for(i = 0; i < document->body.len; i++) {
		fputs(doc_getline(document, i), fp);
		fputc('\n', fp);
	}
	fclose(fp);
	sprintf(p2, "%s.out", p);

	pid = fork();
	if(pid == 0) {
        	printf("  Compiling program...\n");
                execlp("gcc", "gcc", "-ansi", "-Wall", "-Wextra", "-Wpedantic", "-Werror", p, "-o", p2, NULL);
                printf("  Unable to run compiler\n");
                _exit(1);
	} else {
                wait(&i);
        }
	if(WIFEXITED(i) && WEXITSTATUS(i) == 0) {
		printf("  Compilation successful\n");
		sprintf(p, "./%s", p2);
                pid = fork();
                if(pid == 0) {
                	printf("  Executing program...\n");
                        execlp(p, p, NULL);
                        printf("  Unable to execute program\n");
                        _exit(1);
                } else {
                        wait(&i);
                        printf("  Program exited with code %d\n", WEXITSTATUS(i));
			return 1;
                }
	} else {
                printf("  Compilation failed with code %d\n", WEXITSTATUS(i));
        	return -1;
        }
	return -1;
}

int cmd_help(doc *document, char *l) {
	document++;
	if(!l) {
		printf("  Available commands (case insensitive):\n");
                printf("    (1) INSERT\n    (2) DELETE\n    (3) INSERTINLINE\n    (4) REPLACEINLINE\n    (5) DELETEINLINE\n    (6) PRINT\n    (7) INFO\n    (8) RENAME\n    (9) EOF\n    (10) SAVEFILE\n    (11) OPENFILE\n    (12) NP_COMPILERUNC\n");
                printf("  For more details, use HELP [command] (e.g., HELP INSERT)\n                     or HELP [number]  (e.g., HELP 1)\n");
	} else if(strcmp(l, "1") == 0 || strcmp_ci(l, "insert")) {
		printf("  The `INSERT` command is used to write text lines to the document\n  If a line number is provided, then it inserts the text at that line\n  If not, then the text is appended to a new line at the end of the document\n  If no text is provided, an empty line is inserted\n   | INSERT [optional line number] [optional text]\n");
	} else if(strcmp(l, "2") == 0 || strcmp_ci(l, "delete")) {
		printf("  The `DELETE` command deletes lines\n  It takes a starting line and an optional amount of lines to delete (1 if not provided) as parameters\n   | DELETE [line number] [optional line count]\n");
	} else if(strcmp(l, "3") == 0 || strcmp_ci(l, "print")) {
		printf("  The `PRINT` command is used to view the document with line numbers\n  It takes a starting and optional ending line numbers (inclusive) as parameters\n  If the ending line is not provided, it will print the entire document, starting from the line of the first argument\n  If no parameters are provided, it will print the entire document\n   | PRINT [optional starting line] [optional ending line]\n");
	} else if(strcmp(l, "4") == 0 || strcmp_ci(l, "insertinline")) {
		printf("  The `INSERTINLINE` command is used to insert text to an existing line\n  It takes two integer parameters: the line number and the starting index (1-indexed)\n  Any existing text in the line is shifted to make space for the new text\n   | INSERTINLINE [line number] [starting index] [text]\n");
	} else if(strcmp(l, "5") == 0 || strcmp_ci(l, "replaceinline")) {
		printf("  THE `REPLACEINLINE` command is used to write text to an existing line\n  It takes two integer parameters: the line number and the starting index (1-indexed)\n  The new text is written over the existing text\n   | REPLACEINLINE [line number] [starting index] [text]\n");
	} else if(strcmp(l, "6") == 0 || strcmp_ci(l, "deletefromline")) {
		printf("  The `DELETEFROMLINE` command is used to delete text from an existing line\n  It takes three integer parameters: the line number, the starting index (1-indexed), and the number of characters\n   |  DELETEFROMLINE [line number] [starting index] [number of chars]\n");
	} else if(strcmp(l, "7") == 0 || strcmp_ci(l, "info")) {
		printf("  The `INFO` command prints information about the document\n  No parameters are taken\n   | INFO\n");
	} else if(strcmp(l, "8") == 0 || strcmp_ci(l, "rename")) {
		printf("  The `RENAME` command is used to rename the document\n   | RENAME [new title]\n");
        } else if(strcmp(l, "9") == 0 || strcmp_ci(l, "eof")) {
		printf("  The `EOF` command is used to exit the program\n  No parameters are taken\n  Upon exiting, the contents of the document with numbered lines are printed\n   | EOF\n");
	} else if(strcmp(l, "10") == 0 || strcmp_ci(l, "savefile")) {
		printf("  The `SAVEFILE` command is used to write the document to a file\n  The name of the file is provided as an argument\n   | SAVEFILE [file name]\n");
	} else if(strcmp(l, "11") == 0 || strcmp_ci(l, "openfile")) {
		printf("  The `OPENFILE` command is used to read a text file into the editor\n  The file overwrites any existing text in the document\n   | OPENFILE [file name]\n");
        } else if(strcmp(l, "12") == 0 || strcmp_ci(l, "np_compilerunc")) {
		printf("  The `NP_COMPILERUNC` command assumes that the document is an ANSI C program\n  This command takes no arguments\n  It saves the document as a temporary C file, compiles it with the gcc options `ansi` `Wall` `Wextra` `Wpedantic` `Werror`\n  and runs the program (given the compilation succeeds)\n  It does not run the program with any command line parameters\n   | NP_COMPILERUNC\n");
	} else {
		printf("  Unrecognized command. Use HELP for a list of available commands.\n");
        }
	return 1;
}

int cmd_unknown(doc *document, char *s) {
	document++; s++;
	printf("  Unrecognized command. Use HELP for a lits of available commands.\n");
	return 1;
}



