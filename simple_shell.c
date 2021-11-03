#include "shell.h"

/* global variable for ^C handling */
unsigned int sign_flag;

/**
 * sign_handler - handles ^C signal interupt
 * @uuv: unused variable (required for signal function prototype)
 *
 * Return: void
 */
static void sign_handler(int uuv)
{
	(void) uuv;
	if (sign_flag == 0)
		_puts("\n$ ");
	else
		_puts("\n");
}

/**
 * main - main function for the shell
 * @argc: number of arguments passed to main
 * @argv: array of arguments passed to main
 * @environment: array of environment variables
 *
 * Return: 0 or exit status, or ?
 */
int main(int argc __attribute__((unused)), char **argv, char **environment)
{
	size_t len_buffer = 0;
	unsigned int is_pipe = 0, i;
	holds_t holds = {NULL, NULL, NULL, 0, NULL, 0, NULL};

	holds.argv = argv;
	holds.ev = create_ev(environment);
	signal(SIGINT, sign_handler);
	if (!isatty(STDIN_FILENO))
		is_pipe = 1;
	if (is_pipe == 0)
		_puts("$ ");
	sign_flag = 0;
	while (getline(&(holds.buffer), &len_buffer, stdin) != -1)
	{
		sign_flag = 1;
		holds.counter++;
		holds.cmds = tokenize(holds.buffer, ";");
		for (i = 0; holds.cmds && holds.cmds[i] != NULL; i++)
		{
			holds.cl = tokenize(holds.cmds[i], "\n \t\r");
			if (holds.cl && holds.cl[0])
				if (check_for_inborns(&holds) == NULL)
					check_for_path(&holds);
		free(holds.cl);
		}
		free(holds.buffer);
		free(holds.cmds);
		sign_flag = 0;
		if (is_pipe == 0)
			_puts("$ ");
		holds.buffer = NULL;
	}
	if (is_pipe == 0)
		_puts("\n");
	free_ev(holds.ev);
	free(holds.buffer);
	exit(holds.con);
}
