#                                                    ______                 __
#                                                   / __/ /_     __________/ /
#                                                  / /_/ __/    / ___/ ___/ / 
#                                                 / __/ /_     (__  |__  ) /  
#                                                /_/  \__/____/____/____/_/   
#                                             by: acroue/_____/

################################################################################
#                                 PROGRAM NAME                                 #
################################################################################

SERVER_NAME=server
CLIENT_NAME=client

################################################################################
#                                    COLORS                                    #
################################################################################
DEFAULT    = \033[0m
BLACK    = \033[0;30m
RED        = \033[0;31m
GREEN    = \033[0;32m
YELLOW    = \033[0;33m
BLUE    = \033[0;34m
PURPLE    = \033[0;35m
CYAN    = \033[0;36m
BWHITE    = \033[1;37m

################################################################################
#                               COMPILATION FLAGS                              #
################################################################################
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
DEP_FLAGS = -MMD -MP
RM = rm -rf
LIB_FLAGS = --no-print-directory --silent

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Compilation Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~ #


################################################################################
#                                  DIRECTORIES                                 #
################################################################################
SRCS_DIR = srcs
CLIENT_DIR = client
SERVER_DIR = server
COMMON_DIR = common
INCS_DIR = includes
OBJS_DIR = objs

################################################################################
#                                   LIBRARIES                                  #
################################################################################
# LIBS_DIR = libs

# LIBFT_DIR = $(LIBS_DIR)/libft
# LIBFT = $(LIBFT_DIR)/libft.a
# PENELOPE_DIR = $(LIBS_DIR)/penelope
# PENELOPE = $(PENELOPE_DIR)/penelope.a

# LIB :=	$(LIBFT) $(PENELOPE)

################################################################################
#                                    SOURCES                                   #
################################################################################
INCLUDES := $(INCS_DIR)

INCLUDES_FLAGS := $(addprefix -I , $(INCLUDES))

CLIENT_SRC :=	$(addprefix $(CLIENT_DIR)/, \
	client_main.c \
	client_utils.c )

SERVER_SRC := $(addprefix $(SERVER_DIR)/, \
	server_main.c )

COMMON_SRC := $(addprefix $(COMMON_DIR)/, \
	common.c )

CLIENT_OBJ := $(patsubst %.c,$(OBJS_DIR)/%.o,$(CLIENT_SRC))
CLIENT_DEPS := $(patsubst %.c,$(OBJS_DIR)/%.d,$(CLIENT_SRC))

SERVER_OBJ := $(patsubst %.c,$(OBJS_DIR)/%.o,$(SERVER_SRC))
SERVER_DEPS := $(patsubst %.c,$(OBJS_DIR)/%.d,$(SERVER_SRC))

COMMON_OBJ := $(patsubst %.c,$(OBJS_DIR)/%.o,$(COMMON_SRC))
COMMON_DEPS := $(patsubst %.c,$(OBJS_DIR)/%.d,$(COMMON_SRC))

################################################################################
#                                     RULES                                    #
################################################################################
all: fclean $(CLIENT_NAME) $(SERVER_NAME)

$(CLIENT_NAME): $(COMMON_OBJ) $(CLIENT_OBJ)
	@printf "$(GREEN)* Assembling $(BWHITE)%s$(DEFAULT)\n" "$@"
	@$(CC) $(CFLAGS) $(CLIENT_OBJ) $(COMMON_OBJ) $(LIB) $(INCLUDES_FLAGS) -o $@

$(SERVER_NAME): $(COMMON_OBJ) $(SERVER_OBJ)
	@printf "$(GREEN)* Assembling $(BWHITE)%s$(DEFAULT)\n" "$@"
	@$(CC) $(CFLAGS) $(SERVER_OBJ) $(COMMON_OBJ) $(LIB) $(INCLUDES_FLAGS) -o $@

-include %(DEPS)
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@echo "$<" | grep -q "common" && \
 		printf "$(PURPLE)- Compiling$(DEFAULT) $(BWHITE)common$(DEFAULT) %s\n" "$<" || \
 		printf "$(CYAN)- Compiling$(DEFAULT) %s\n" "$<"
	@cd $(SRCS_DIR) && find . -type d -exec mkdir -p ../$(OBJS_DIR)/{} \; && cd ..
	@$(CC) $(DEP_FLAGS) $(COMPIL_DEFINES) $(CFLAGS) $(INCLUDES_FLAGS) -c $< -o $@

clean:
	@printf "$(RED)! Removing$(DEFAULT) ${OBJS_DIR} files\n"
	@${RM} ${OBJS_DIR}

fclean: clean
	@printf "$(RED)! Removing$(DEFAULT) %s\n" "$(CLIENT_NAME) $(SERVER_NAME)"
	@$(RM) $(CLIENT_NAME) #(SERVER_NAME)

re: fclean all

cre:
	@clear
	@make re --no-print-directory

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Lib Rules ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
$(LIBFT):
	@printf "$(CYAN)~ Compiling$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(LIBFT_DIR)"
	@make -C $(LIBFT_DIR) $(LIB_FLAGS)

$(PENELOPE):
	@printf "$(CYAN)~ Compiling$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(PENELOPE_DIR)"
	@make -C $(PENELOPE_DIR) $(LIB_FLAGS)

cleanlib:
	@printf "$(RED)! Removing$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(LIBFT_DIR)"
	@make clean -C $(LIBFT_DIR) $(LIB_FLAGS)
	@printf "$(RED)! Removing$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(PENELOPE_DIR)"
	@make clean -C $(PENELOPE_DIR) $(LIB_FLAGS)

fcleanlib:
	@printf "$(RED)! Removing$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(LIBFT_DIR)"
	@make fclean -C $(LIBFT_DIR) $(LIB_FLAGS)
	@printf "$(RED)! Removing$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(PENELOPE_DIR)"
	@make fclean -C $(PENELOPE_DIR) $(LIB_FLAGS)

relib: fcleanlib $(LIBFT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Penelope Rules ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
debug: fclean
	@$(MAKE) LOG_VALUE=P_LOG_DEBUG $(LIB_FLAGS)

trace: fclean
	@$(MAKE) LOG_VALUE=P_LOG_TRACE $(LIB_FLAGS)

info: fclean
	@$(MAKE) LOG_VALUE=P_LOG_INFO $(LIB_FLAGS)
