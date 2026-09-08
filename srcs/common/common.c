#include "project.h"

void fatal_error(const char *error_message)
{
	perror(error_message);
	exit(errno);
}

static void read_namecard(t_namecard *namecard)
{
	printf("name : %s\n", namecard->name);
}

static void read_value(int val)
{
	printf("value : %d\n", val);
}

void read_packet(t_packet *packet)
{
	switch (packet->type) {
	case NAMECARD:
		t_namecard *namecard = &packet->data.namecard;
		read_namecard(namecard);
		break;

	case VAL:
		read_value(packet->data.val);
		break;

	default:
		break;
	}
}

void common()
{
	printf("common linking successful\n");
}
