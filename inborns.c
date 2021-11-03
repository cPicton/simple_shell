#include "shell.h"

/**
 * check_for_inborns - checks if the cmd is a inborns
 * @holds: holders
 * Return: pointer to the function or NULL
 */
void (*check_for_inborns(holds_t *holds))(holds_t *holds)
{
	unsigned int i;
	inborns_t check[] = {
		{"exit", new_exit},
		{"ev", _ev},
		{"setev", new_setev},
		{"unsetev", new_unsetev},
		{NULL, NULL}
	};

	for (i = 0; check[i].func != NULL; i++)
	{
		if (_strcmpr(holds->cl[0], check[i].name) == 0)
			break;
	}
	if (check[i].func != NULL)
		check[i].func(holds);
	return (check[i].func);
}

/**
 * new_exit - exit program
 * @holds: holders
 * Return: void
 */

void new_exit(holds_t *holds)
{
	int con;

	if (_strcmpr(holds->cl[0], "exit") == 0 && holds->cl[1] != NULL)
	{
		con = _atoi(holds->cl[1]);
		if (con == -1)
		{
			holds->con = 2;
			print_error(holds, ": Invalid Number: ");
			_puts2(holds->cl[1]);
			_puts("\n");
			free(holds->cmds);
			holds->cmds = NULL;
			return;
		}
		holds->con = con;
	}
	free(holds->buffer);
	free(holds->cl);
	free(holds->cmds);
	free_ev(holds->ev);
	exit(holds->con);
}

/**
 * _ev - prints the current enviro
 * @holds: struct of holders
 * Return: void
 */

void _ev(holds_t *holds)
{
	unsigned int i;

	for (i = 0; holds->ev[i]; i++)
	{
		_puts(holds->ev[i]);
		_puts("\n");
	}
	holds->con = 0;
}

/**
 * new_setev - create a new enviro holder, or edit an existing one
 * @holds: pointer to struct of holders
 * Return: void
 */

void new_setev(holds_t *holds)
{
	char **key;
	char *hold;

	if (holds->cl[1] == NULL || holds->cl[2] == NULL)
	{
		print_error(holds, ": Incorrect number of arguments\n");
		holds->con = 2;
		return;
	}
	key = find_key(holds->ev, holds->cl[1]);
	if (key == NULL)
		append_key(holds);
	else
	{
		hold = add_value(holds->cl[1], holds->cl[2]);
		if (hold == NULL)
		{
			print_error(holds, NULL);
			free(holds->buffer);
			free(holds->cmds);
			free(holds->cl);
			free_ev(holds->ev);
			exit(127);
		}
		free(*key);
		*key = hold;
	}
	holds->con = 0;
}

/**
 * new_unsetev - remove an enviro holder
 * @holds: pointer to a struct of holders
 * Return: void
 */

void new_unsetev(holds_t *holds)
{
	char **key, **newev;
	unsigned int i, j;

	if (holds->cl[1] == NULL)
	{
		print_error(holds, ": Incorrect number of arguments\n");
		holds->con = 2;
		return;
	}
	key = find_key(holds->ev, holds->cl[1]);
	if (key == NULL)
	{
		print_error(holds, ": No variable to unset");
		return;
	}
	for (i = 0; holds->ev[i] != NULL; i++)
		;
	newev = malloc(sizeof(char *) * i);
	if (newev == NULL)
	{
		print_error(holds, NULL);
		holds->con = 127;
		new_exit(holds);
	}
	for (i = 0; holds->ev[i] != *key; i++)
		newev[i] = holds->ev[i];
	for (j = i + 1; holds->ev[j] != NULL; j++, i++)
		newev[i] = holds->ev[j];
	newev[i] = NULL;
	free(*key);
	free(holds->ev);
	holds->ev = newev;
	holds->con = 0;
}
