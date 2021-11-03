#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <signal.h>

/**
 * struct holders - holders
 * @cl: command line
 * @buffer: buffer
 * @ev: enviroment variables
 * @counter: count entered commands
 * @argv: argument at opening
 * @con: exit condition
 * @cmds: commands to be excuted
 */

typedef struct holders
{
	char **cl;
	char *buffer;
	char **ev;
	size_t counter;
	char **argv;
	int con;
	char **cmds;
} holds_t;

/**
 * struct inborn - struct for the native func
 * @name: name of inborn command
 * @func: function for corresponding inborn
 */

typedef struct inborns
{
	char *name;
	void (*func)(holds_t *);
} inborns_t;

char **create_ev(char **ev);
void free_ev(char **ev);

ssize_t _puts(char *str);
char *_strdup(char *strtodup);
int _strcmpr(char *strcmp1, char *strcmp2);
char *_strcat(char *strc1, char *strc2);
unsigned int _strlen(char *str);

char **tokenize(char *buffer, char *delimiter);
char **_realloc(char **ptr, size_t *size);
char *new_strtok(char *str, const char *delim);

void (*check_for_inborns(holds_t *holds))(holds_t *holds);
void new_exit(holds_t *holds);
void _ev(holds_t *holds);
void new_setev(holds_t *holds);
void new_unsetev(holds_t *holds);

void append_key(holds_t *holds);
char **find_key(char **ev, char *key);
char *add_value(char *key, char *value);
int _atoi(char *str);

void check_for_path(holds_t *holds);
int path_execute(char *cmd, holds_t *holds);
char *find_path(char **ev);
int execute_cwd(holds_t *holds);
int check_for_dir(char *str);

void print_error(holds_t *holds, char *msg);
void _puts2(char *str);
char *_uitoa(unsigned int count);

#endif
