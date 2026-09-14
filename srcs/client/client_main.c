#include "project.h"
#include "client.h"

const char command_names[8][10] = {
	"q",
	"quit",
	"s",
	"send",
	"v",
	"val",
	"",
};

void loop(t_net *network)
{
	int	  alive = true;
	char *buffer = NULL;

	while (alive) {
		printf("Commands : q(uit), s(end), v(al)\n> ");
		buffer = getstr(buffer);
		if (handle_cmd(network, buffer) == false)
			break;
	}
}

int main(int argc, const char *argv[])
{
	printf("Starting client\n");
	t_net network = {0};

	if (establish_connection(&network) == -1)
		return 1;

	loop(&network);

	close_client(&network);

	(void)argc;
	(void)argv;
	return 0;
}
