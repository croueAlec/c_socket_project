#include "project.h"
#include "client.h"

bool		 logged_in = false;
bool		 is_player_1 = false;
t_game_state state = {0};

void loop(t_net *network)
{
	int	 alive = true;
	char buffer[BUFFER_SIZE + 1] = {0};

	while (alive) {
		bzero(buffer, BUFFER_SIZE);
		printf("Commands : q(uit), a(ction), l(ogin)\n> ");
		get_user_string(network, buffer, BUFFER_SIZE);
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
