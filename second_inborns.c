#include "shell.h"

/**
 * append_key - create a new enviro holders
 * @holds: pointer to struct of holders
 * Return: void
 */

void append_key(holds_t *holds)
{
	unsigned int i;
	char **newev;

	for (i = 0; holds->ev[i] != NULL; i++)
		;
	newev = malloc(sizeof(char *) * (i + 2));
	if (newev == NULL)
	{
		print_error(holds, NULL);
		holds->con = 127;
		new_exit(holds);
	}
	for (i = 0; holds->ev[i] != NULL; i++)
		newev[i] = holds->ev[i];
	newev[i] = add_value(holds->cl[1], holds->cl[2]);
	if (newev[i] == NULL)
	{
		print_error(holds, NULL);
		free(holds->buffer);
		free(holds->cmds);
		free(holds->cl);
		free_ev(holds->ev);
		free(newev);
		exit(127);
	}
	newev[i + 1] = NULL;
	free(holds->ev);
	holds->ev = newev;
}

/**
 * find_key - finds an enviro holders
 * @ev: array of enviro holders
 * @key: enviro holders to find
 *
 * Return: pointer to address of the enviro holders
 */
char **find_key(char **ev, char *key)
{
	unsigned int i, j, len;

	len = _strlen(key);
	for (i = 0; ev[i] != NULL; i++)
	{
		for (j = 0; j < len; j++)
			if (key[j] != ev[i][j])
				break;
		if (j == len && ev[i][j] == '=')
			return (&ev[i]);
	}
	return (NULL);
}

/**
 * add_value - create a new enviro holder string
 * @key: holder name
 * @value: holder value
 *
 * Return: pointer to the new string;
 */
char *add_value(char *key, char *value)
{
	unsigned int len1, len2, i, j;
	char *new;

	len1 = _strlen(key);
	len2 = _strlen(value);
	new = malloc(sizeof(char) * (len1 + len2 + 2));
	if (new == NULL)
		return (NULL);
	for (i = 0; key[i] != '\0'; i++)
		new[i] = key[i];
	new[i] = '=';
	for (j = 0; value[j] != '\0'; j++)
		new[i + 1 + j] = value[j];
	new[i + 1 + j] = '\0';
	return (new);
}

/**
 * _atoi - converts a string into an integer
 * @str: string to convert
 *
 * Return: the integer value, or -1 if an error occurs
 */
int _atoi(char *str)
{
	unsigned int i, digits;
	int num = 0, num_test;

	num_test = INT_MAX;
	for (digits = 0; num_test != 0; digits++)
		num_test /= 10;
	for (i = 0; str[i] != '\0' && i < digits; i++)
	{
		num *= 10;
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		if ((i == digits - 1) && (str[i] - '0' > INT_MAX % 10))
			return (-1);
		num += str[i] - '0';
		if ((i == digits - 2) && (str[i + 1] != '\0') && (num > INT_MAX / 10))
			return (-1);
	}
	if (i > digits)
		return (-1);
	return (num);
}

