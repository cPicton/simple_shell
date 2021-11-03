#include "shell.h"

/**
 * path_execute - executes a command in the path
 * @cmd: full path to the command
 * @holds: pointer to struct of holder
 *
 * Return: 0 on succcess, 1 on failure
 */
int path_execute(char *cmd, holds_t *holds)
{
	pid_t child_pid;

	if (access(cmd, X_OK) == 0)
	{
		child_pid = fork();
		if (child_pid == -1)
			print_error(holds, NULL);
		if (child_pid == 0)
		{
			if (execve(cmd, holds->cl, holds->ev) == -1)
				print_error(holds, NULL);
		}
		else
		{
			wait(&holds->con);
			if (WIFEXITED(holds->con))
				holds->con = WEXITSTATUS(holds->con);
			else if (WIFSIGNALED(holds->con) && WTERMSIG(holds->con) == SIGINT)
				holds->con = 130;
			return (0);
		}
		holds->con = 127;
		return (1);
	}
	else
	{
		print_error(holds, ": Permission denied\n");
		holds->con = 126;
	}
	return (0);
}

/**
 * find_path - finds the PATH variable
 * @ev: array of environment variables
 *
 * Return: pointer to the node that contains the PATH, or NULL on failure
 */
char *find_path(char **ev)
{
	char *path = "PATH=";
	unsigned int i, j;

	for (i = 0; ev[i] != NULL; i++)
	{
		for (j = 0; j < 5; j++)
			if (path[j] != ev[i][j])
				break;
		if (j == 5)
			break;
	}
	return (ev[i]);

}

/**
 * check_for_path - checks if the command is in the PATH
 * @holds: holders
 *
 * Return: void
 */
void check_for_path(holds_t *holds)
{
	char *path, *path_dup = NULL, *check = NULL;
	unsigned int i = 0, r = 0;
	char **path_tokens;
	struct stat buf;

	if (check_for_dir(holds->cl[0]))
		r = execute_cwd(holds);
	else
	{
		path = find_path(holds->ev);
		if (path != NULL)
		{
			path_dup = _strdup(path + 5);
			path_tokens = tokenize(path_dup, ":");
			for (i = 0; path_tokens && path_tokens[i]; i++, free(check))
			{
				check = _strcat(path_tokens[i], holds->cl[0]);
				if (stat(check, &buf) == 0)
				{
					r = path_execute(check, holds);
					free(check);
					break;
				}
			}
			free(path_dup);
			if (path_tokens == NULL)
			{
				holds->con = 127;
				new_exit(holds);
			}
		}
		if (path == NULL || path_tokens[i] == NULL)
		{
			print_error(holds, ": not found\n");
			holds->con = 127;
		}
		free(path_tokens);
	}
	if (r == 1)
		new_exit(holds);
}

/**
 * execute_cwd - executes the command in the current working directory
 * @holds: pointer to struct of holders
 *
 * Return: 0 on success, 1 on failure
 */
int execute_cwd(holds_t *holds)
{
	pid_t child_pid;
	struct stat buf;

	if (stat(holds->cl[0], &buf) == 0)
	{
		if (access(holds->cl[0], X_OK) == 0)
		{
			child_pid = fork();
			if (child_pid == -1)
				print_error(holds, NULL);
			if (child_pid == 0)
			{
				if (execve(holds->cl[0], holds->cl, holds->ev) == -1)
					print_error(holds, NULL);
			}
			else
			{
				wait(&holds->con);
				if (WIFEXITED(holds->con))
					holds->con = WEXITSTATUS(holds->con);
				else if (WIFSIGNALED(holds->con) && WTERMSIG(holds->con) == SIGINT)
					holds->con = 130;
				return (0);
			}
			holds->con = 127;
			return (1);
		}
		else
		{
			print_error(holds, ": Permission denied\n");
			holds->con = 126;
		}
			return (0);
	}
	print_error(holds, ": not found\n");
	holds->con = 127;
	return (0);
}

/**
 * check_for_dir - checks if the command is a part of a path
 * @str: command
 *
 * Return: 1 on success, 0 on failure
 */
int check_for_dir(char *str)
{
	unsigned int i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '/')
			return (1);
	}
	return (0);
}
