#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "cll.h"

int readline(char **b, size_t *size) {
	size_t i = 0; 
	int c = getchar();
	while(c != '\n' && c != EOF) {
		if(i + 1 >= *size) {
			 *b = realloc(*b, *size *= 2);
		}
		(*b)[i++] = c;
		c = getchar();
	}
	(*b)[i] = '\0';
	return c != EOF || i != 0;
}

int main() {
	cll doc = cll_create();
	char *b = malloc(16), *p, *p2;
	int i, k = 1, arg1, arg2, t1, t2, t3;
	size_t bsize = 16, blen;
	char *tok;
	FILE *fp;
	time_t t;
	pid_t pid;
	
	while(k) {
		printf("> ");
		k = readline(&b, &bsize);

		blen = strlen(b);
		tok = strtok(b, " ");
		if(!tok) {
			continue;
		} else if(strcmp(tok, "INSERT") == 0) {
			tok = strtok(NULL, " ");

			if(!tok) {
				cll_insert(&doc, -1, "");
				continue;
			}
			arg1 = 0;
			for(i = 0; tok[i] != '\0'; i++) {
				if(!isdigit(tok[i])) {		
					if(blen > strlen(b) + strlen(tok) + 1) {
						tok[strlen(tok)] = ' ';
					}

					cll_insert(&doc, -1, tok);
					arg1 = -1;
					break;
				}
			}
			if(arg1 == -1) {
				continue;
			}

			arg1 = atoi(tok);
			if(arg1 <= 0) {
				printf("  Invalid line number %d.\n", arg1);
				continue;
			}

			while(doc.len < arg1 - 1) {
				cll_insert(&doc, -1, "");
			}
			
			arg2 = strlen(tok);	
			tok = strtok(NULL, " ");
			if(!tok) {
				cll_insert(&doc, arg1 - 1, "");
				continue;
			}

			if(blen > strlen(b) + arg2 + strlen(tok) + 2) {
				tok[strlen(tok)] = ' ';
			}
			cll_insert(&doc, arg1 - 1, tok);
		} else if(strcmp(tok, "EOF") == 0) {
			break;
		} else if(strcmp(tok, "DELETE") == 0) {
			tok = strtok(NULL, " ");
			if(!tok) {
				printf("  Missing required line number.\n");
                                continue;
                        }
			
			arg1 = 0;
                        for(i = 0; tok[i] != '\0'; i++) {
                                if(!isdigit(tok[i])) {
                                        printf("  %s is not a valid number\n", tok);
                                        arg1 = -1;
                                	break;
				}
                        }
			if(arg1 == -1) {
				continue;
			}
                        arg1 = atoi(tok);
			
			arg2 = 1;
			
			tok = strtok(NULL, " ");
			if(tok) {
				for(i = 0; tok[i] != '\0'; i++) {
                        	        if(!isdigit(tok[i])) {
                        	                printf("  %s is not a valid number\n", tok);
                        	                arg2 = -1;
						break;
                        	        }
                        	}
                        	arg2 = atoi(tok);
			}
			if(arg2 == -1) {
				continue;
			}

			if(arg1 < 1 || arg1 > doc.len) {
				printf("  Invalid line number\n");
				continue;
			}
			while(arg2-- > 0 && doc.len >= arg1) {
				free(cll_remove(&doc, arg1 - 1));
			}
		} else if(strcmp(tok, "PRINT") == 0) {
			tok = strtok(NULL, " ");
			if(!tok) {
				arg1 = 1;
			} else {
				for(i = 0; tok[i] != '\0'; i++) {
					if(!isdigit(tok[i])) {
						printf("  %s is not a valid number\n", tok);
						continue;
					}
				}	
				arg1 = atoi(tok);
				tok = strtok(NULL, " ");
			}
			
			if(!tok) {
				arg2 = doc.len;
			} else {
				for(i = 0; tok[i] != '\0'; i++) {
					if(!isdigit(tok[i])) {
						printf("  %s is not a valid number\n", tok);
						continue;
					}
				}
				arg2 = atoi(tok);
			}

			if(arg1 > doc.len || arg2 > doc.len || arg1 <= 0) {
				printf("  Line number is out of bounds.\n");
				continue;
			}
			
			if(arg1 > arg2) {
				printf("  Start line number must be less than or equal to end line number.\n");
				continue;
			}
			
			t1 = doc.len;
        		t2 = 1;

        		while((t1 /= 10) > 0) {
                		t2++;
        		}

			for(i = arg1 - 1; i < arg2; i++) {
               			printf("%*d |%s\n", t2, i + 1, cll_gets(&doc, i));
        		}
		} else if(strcmp(tok, "SAVEFILE") == 0) {
			tok = strtok(NULL, " ");
			if(!tok) {
				printf("  Missing file name.\n");
				continue;
			}
			
			if(blen > strlen(b) + strlen(tok) + 1) {
				tok[strlen(tok)] = ' ';
			}

			fp = fopen(tok, "wx");

			if(!fp) {
				printf("  File already exists.\n");
				continue;
			}
			for(i = 0; i < doc.len; i++) {
                		fputs(cll_gets(&doc, i), fp);
				fputc('\n', fp);
        		}
			fclose(fp);
		} else if(strcmp(tok, "OPENFILE") == 0) {
			tok = strtok(NULL, " ");

			if(!tok) {
				printf("  Missing file name.\n");
				continue;
			}

			if(blen > strlen(b) + strlen(tok) + 1) {
				tok[strlen(tok)] = ' ';
			}

			fp = fopen(tok, "r");
			
			if(!fp) {
				printf("  Invalid file name.\n");
				continue;
			}

			p = malloc(16);
			t1 = 16;
			t3 = 0;


			while(doc.len > 0) {
				free(cll_remove(&doc, 0));
			}

			while((t2 = fgetc(fp)) != EOF) {
				if(t2 == '\n') {
					p[t3] = '\0';
					cll_insert(&doc, -1, p);
					t3 = 0;
					continue;
				} 
				if(t3 + 1 >= t1) {
					p = realloc(p, t1 *= 2);
				}
				p[t3++] = t2;
			}
			if(t3 != 0) {
				p[t3] = '\0';
				cll_insert(&doc, -1, p);
			}
			free(p);
			fclose(fp);
		} else if(strcmp(tok, "NP_COMPILERUNC") == 0) {
			t = time(NULL);
			p = malloc(50);
			p2 = malloc(50);	
			sprintf(p, "temp%ld.c", (long)t);

                        fp = fopen(p, "wx");

			if(!fp) {
				printf("  Unable to create temp file.\n");
				continue;
			}
                        for(i = 0; i < doc.len; i++) {
                                fputs(cll_gets(&doc, i), fp);
                                fputc('\n', fp);
                        }
                        fclose(fp);

			pid = fork();
			sprintf(p2, "%s.out", p);
			if(pid == 0) {
				printf("  Compiling program...\n");
				execlp("gcc", "gcc", "-ansi", "-Wall", "-Wextra", "-Wpedantic", "-Werror", p, "-o", p2, NULL);
				printf("  Unable to run compiler\n");
				_exit(1);
			} else {
				wait(&t1);
			}
			if(WIFEXITED(t1) && WEXITSTATUS(t1) == 0) {
				printf("  Compilation successful\n");
				sprintf(p, "./%s", p2);
				pid = fork();
				if(pid == 0) {
					printf("  Executing program...\n");
					execlp(p, p, NULL);
					printf("  Unable to execute program\n");
					_exit(1);
				} else {
					wait(&t1);
					printf("\n  Program exited with code %d\n", WEXITSTATUS(t1));
				}
			} else {
				printf("  Compilation failed with code %d\n", WEXITSTATUS(t1));
				continue;
			}
			
			free(p);
			free(p2);
		} else if(strcmp(tok, "HELP") == 0) {
			tok = strtok(NULL, " ");
			if(!tok) {
				printf("  Available commands (case sensitive):\n");
				printf("    (1) INSERT\n    (2) DELETE\n    (3) PRINT\n    (4) EOF\n    (5) SAVEFILE\n    (6) OPENFILE\n    (7) NP_COMPILERUNC\n");
				printf("  For more details, use HELP [command] (e.g., HELP INSERT)\n                     or HELP [number]  (e.g., HELP 1)\n");
			} else if(strcmp(tok, "1") == 0 || strcmp(tok, "INSERT") == 0) {
				printf("  The `INSERT` command is used to write text lines to the document\n  If a line number is provided, then it inserts the text at that line\n  If not, then the text is appended to a new line at the end of the document\n  If no text is provided, an empty line is inserted\n   | INSERT [optional line number] [optional text]\n");
			} else if(strcmp(tok, "2") == 0 || strcmp(tok, "DELETE") == 0) {
				printf("  The `DELETE` command deletes lines\n  It takes a starting line and an optional amount of lines to delete (1 if not provided) as parameters\n   | DELETE [line number] [optional line count]\n");
			} else if(strcmp(tok, "3") == 0 || strcmp(tok, "PRINT") == 0) {
				printf("  The `PRINT` command is used to view the document with line numbers\n  It takes a starting and optional ending line numbers (inclusive) as parameters\n  If the ending line is not provided, it will print the entire document, starting from the line of the first argument\n  If no parameters are provided, it will print the entire document\n   | PRINT [optional starting line] [optional ending line]\n");

			} else if(strcmp(tok, "4") == 0 || strcmp(tok, "EOF") == 0) {
				printf("  The `EOF` command is used to exit the program\n  No parameters are taken\n  Upon exiting, the contents of the document with numbered lines are printed\n");

			} else if(strcmp(tok, "5") == 0 || strcmp(tok, "SAVEFILE") == 0) {
				printf("  The `SAVEFILE` command is used to write the document to a file\n  The name of the file is provided as an argument\n   | SAVEFILE [file name]\n");

			} else if(strcmp(tok, "6") == 0 || strcmp(tok, "OPENFILE") == 0) {
				printf("  The `OPENFILE` command is used to read a text file into the editor\n  The file overwrites any existing text in the document\n   | OPENFILE [file name]\n");

			} else if(strcmp(tok, "7") == 0 || strcmp(tok, "NP_COMPILERUNC") == 0) {
				printf("  The `NP_COMPILERUNC` command assumes that the document is an ANSI C program\n  This command takes no arguments\n  It saves the document as a temporary C file, compiles it with the gcc options `ansi` `Wall` `Wextra` `Wpedantic` `Werror`\n  and runs the program (given the compilation succeeds)\n  It does not run the program with any command line parameters\n   | NP_COMPILERUNC\n");
			} else {
				printf("  Invalid command. Use HELP for a list of available commands.\n");
			}
		} else {
			printf("  Invalid command. Use HELP for a list of available commands.\n");
		}
	}
	t1 = doc.len;
	t2 = 1;
	while((t1 /= 10) > 0) {
		t2++;
	}
	printf("\n----------Document Contents----------\n");
	for(i = 0; i < doc.len; i++) {
		printf("%*d |%s\n", t2, i + 1, cll_gets(&doc, i));
	}
	return 0;
}

