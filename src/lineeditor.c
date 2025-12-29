#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "cll.h"

int readline(char **b, int *size) {
	int i = 0, c = getchar();
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
	char *s = malloc(16), *p, *p2;
	int l = 16, i, d, t1, t2, t3;
	char command[16];
	FILE *fp;
	time_t t;
	pid_t pid;

	while(readline(&s, &l)) {
		command[0] = '\0';
		sscanf(s, "%15s", command);

		if(strcmp(command, "INSERT") == 0) {
			if(strlen(s) <= 6) {
				cll_insert(&doc, -1, "");
				continue;
			}
			p = s + 7;
			sscanf(p, "%15s", command);
			for(i = 0; *(command + i) != '\0'; i++) {
				if(!isdigit(*(command + i))) {
					goto insert_append;
				}
			}
			d = atoi(command);
			if(d <= 0) {
				printf("Invalid line number.");
				continue;
			}
			while(doc.len < d - 1) {
				cll_insert(&doc, -1, "");
			}
			
			while(*p == ' ' || *p == '\t' || *p == '\n') {
				p++;
			}

			p += strlen(command);

                        while(*p == ' ' || *p == '\t' || *p == '\n') {
				p++;
			}
			
			cll_insert(&doc, d - 1, p);

			continue; 
			insert_append:
			cll_insert(&doc, -1, p);
		} if(strcmp(command, "EOF") == 0) {
			break;
		} else if(strcmp(command, "DELETE") == 0) {
			t2 = 1;
			t1 = -1;
			if(strlen(s) <= 6) {
				printf("No parameters");
                                continue;
                        }
			p = s + 6;
                        sscanf(p, "%15s", command);
                        for(i = 0; *(command + i) != '\0'; i++) {
                                if(!isdigit(*(command + i))) {
                                        printf("%s is not a valid number", command);
                                        continue;
                                }
                        }
                        t1 = atoi(command);

                        while(*p == ' ' || *p == '\t' || *p == '\n') {
                                p++;
                        }

                        p += strlen(command);

                        while(*p == ' ' || *p == '\t' || *p == '\n') {
                                p++;
                        }

                        if(*p == '\0') {
                                goto delete_oneparam;
                        }
                        sscanf(p, "%15s", command);
                        for(i = 0; *(command + i) != '\0'; i++) {
                                if(!isdigit(*(command + i))) {
                                        printf("%s is not a valid number\n", command);
                                        continue;
                                }
                        }
                        t2 = atoi(command);

			delete_oneparam:
			if(t1 < 1 || t1 > doc.len) {
				printf("Invalid line number\n");
				continue;
			}
			while(t2-- > 0 && doc.len >= t1) {
				free(cll_remove(&doc, t1 - 1));
			}		
		} else if(strcmp(command, "PRINT") == 0) {
			d = doc.len;
			t3 = 1;
			if(strlen(s) <= 5) {
				goto print_noparam;
			}

			p = s + 5;
                        sscanf(p, "%15s", command);
                        for(i = 0; *(command + i) != '\0'; i++) {
                                if(!isdigit(*(command + i))) {
					printf("%s is not a valid number", command);
                                        continue;
                                }
                        }
                        t3 = atoi(command);

			while(*p == ' ' || *p == '\t' || *p == '\n') {
                                p++;
                        }

                        p += strlen(command);

                        while(*p == ' ' || *p == '\t' || *p == '\n') {
                                p++;
                        }

			if(*p == '\0') {
				goto print_noparam;
			}
			sscanf(p, "%15s", command);
                        for(i = 0; *(command + i) != '\0'; i++) {
                                if(!isdigit(*(command + i))) {
					printf("%s is not a valid number\n", command);
                                        continue;
                                }
                        }

			d = atoi(command);

			print_noparam:	
			if(t3 > doc.len || d > doc.len || t3 == 0) {
				printf("Line number is out of bounds.\n");
				continue;
			}
			
			if(t3 > d) {
				printf("Start line number must be less than or equal to end line number.\n");
				continue;
			}
			
			t1 = doc.len;
        		t2 = 1;
        		while((t1 /= 10) > 0) {
                		t2++;
        		}

			for(i = t3 - 1; i < d; i++) {
               			printf("%*d |%s\n", t2, i + 1, cll_gets(&doc, i));
        		}
		} else if(strcmp(command, "SAVEFILE") == 0) {
			strtok(s, " \t");
			p = strtok(NULL, " \t");
			if(!p) {
				printf("Missing file name.\n");
				continue;
			}
			
			fp = fopen(p, "wx");

			if(!fp) {
				printf("File already exists.\n");
				continue;
			}
			for(i = 0; i < doc.len; i++) {
                		fputs(cll_gets(&doc, i), fp);
				fputc('\n', fp);
        		}
			fclose(fp);
		} else if(strcmp(command, "OPENFILE") == 0) {
			strtok(s, " \t");
			p = strtok(NULL, " \t");

			if(!p) {
				printf("Missing file name.\n");
				continue;
			}

			fp = fopen(p, "r");
			
			if(!fp) {
				printf("Invalid file name.\n");
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
		} else if(strcmp(command, "NP_COMPILERUNC") == 0) {
			t = time(NULL);
			p = malloc(50);
			p2 = malloc(50);	
			sprintf(p, "temp%ld.c", (long)t);

                        fp = fopen(p, "wx");

			if(!fp) {
				printf("something went wrong\n");
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
				execlp("gcc", "gcc", "-ansi", "-Wall", "-Wextra", "-Wpedantic", "-Werror", p, "-o", p2, NULL);
				printf("something went wrong\n");
				_exit(1);
			}
			
			wait(&t1);
			if(WIFEXITED(t1) && WEXITSTATUS(t1) == 0) {
				printf("program compiled.\n");
				sprintf(p, "./%s", p2);
				pid = fork();
				if(pid == 0) {
					printf("executing program...\n");
					execlp(p, p, NULL);
					printf("something went wrong\n");
					_exit(1);
				}
				wait(&t1);
				printf("\n\nprogram exited with code %d\n", WEXITSTATUS(t1));
			} else {
				printf("something went wrong\n");
				continue;
			}
			
			free(p);
			free(p2);
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







