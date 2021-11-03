#include "shell.h"

/**
 * create_ev - make the shell enviro from the enviro passed to main
 * @ev: enviro passed to main
 *
 * Return: pointer to the new enviro
 */
char **create_ev(char **ev)
{
	char **newev = NULL;
	size_t i;

	for (i = 0; ev[i] != NULL; i++)
		;
	newev = malloc(sizeof(char *) * (i + 1));
	if (newev == NULL)
	{
		perror("Fatal Error");
		exit(1);
	}
	for (i = 0; ev[i] != NULL; i++)
		newev[i] = _strdup(ev[i]);
	newev[i] = NULL;
	return (newev);
}

/**
 * free_ev - free the shell's enviro
 * @ev: shell's enviro
 *
 * Return: void
 */
void free_ev(char **ev)
{
	unsigned int i;

	for (i = 0; ev[i] != NULL; i++)
		free(ev[i]);
	free(ev);
}
