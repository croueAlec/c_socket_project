SERVER_NAME=server
CLIENT_NAME=client

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

all: clean client_rule server_rule

client_rule:
	@rm -rf $(CLIENT_NAME)
	@$(CC) $(CFLAGS) client_main.c -o $(CLIENT_NAME)

server_rule:
	@rm -rf $(SERVER_NAME)
	@$(CC) $(CFLAGS) server_main.c -o $(SERVER_NAME)

clean:
	@rm -rf client server
