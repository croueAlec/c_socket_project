#include "project.h"

void fatal_error(const char *error_message)
{
	perror(error_message);
	exit(errno);
}
