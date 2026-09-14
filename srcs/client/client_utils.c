#include "project.h"
#include "client.h"

static void lowercase_string(char *str)
{
	for (size_t i = 0; str && str[i]; i++) {
		str[i] = tolower(str[i]);
	}
}

static bool trim(char *str)
{
	size_t i = 0;
	for (; str[i] && isspace(str[i]); i++) {
		str[i] = ' ';
	}

	char *beginning = strchr(str, ' ');
	if (beginning == NULL || strlen(beginning) <= 1)
		return false;

	beginning++;
	char *end = strchr(beginning, ' ');
	*end = '\0';

	memmove(str, beginning, strlen(beginning));
	return true;
}

void clean_string(char *str)
{
	if (strlen(str) == 0)
		return;

	if (trim(str) == false)
		return;

	lowercase_string(str);
}
