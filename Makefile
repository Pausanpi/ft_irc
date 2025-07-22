NAME = ircserver
INCLUDE = includes
SRC_DIR = src
OBJ_DIR = obj
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
AR = ar rcs

##########################################################################

DEF_COLOR = $(shell tput sgr0)
GRAY = $(shell tput setaf 0)
RED = $(shell tput setaf 1)
GREEN = $(shell tput setaf 2)
YELLOW = $(shell tput setaf 3)
BLUE = $(shell tput setaf 4)
MAGENTA = $(shell tput setaf 5)
CYAN = $(shell tput setaf 6)
WHITE = $(shell tput setaf 7)

#########################################################################

MAIN_FILES = main.cpp

ENTITIES_FILES = Server.cpp Client.cpp Channel.cpp

COMMANDS_FILES = CommandHandler.cpp Join.cpp NickUser.cpp Privmsg.cpp \
				Quit.cpp Mode.cpp Kick.cpp Invite.cpp Pass.cpp Topic.cpp

ENTITIES_FULL = $(addprefix entities/, $(ENTITIES_FILES))
COMMANDS_FULL = $(addprefix commands/, $(COMMANDS_FILES))

SRC_FILES = $(MAIN_FILES) $(ENTITIES_FULL) $(COMMANDS_FULL)

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.o))

###########################################################################

all: $(NAME)

$(NAME): $(OBJ)
			@$(CC) -I./$(INCLUDE) $(CFLAGS) $(OBJ) -o $(NAME) 
			@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
			@mkdir -p $(@D)
			@echo "$(BLUE)Created directory: $(@D)$(DEF_COLOR)"
			@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"
			@$(CC) -I./$(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
			@rm -rf $(OBJ_DIR)
			@echo "$(BLUE)$(NAME) object files cleaned!$(DEF_COLOR)"

fclean:		clean
			@rm -f $(NAME)
			@echo "$(BLUE)$(NAME) executable cleaned!$(DEF_COLOR)"

re:			fclean all
			@echo "$(MAGENTA)$(NAME) recompiled!$(DEF_COLOR)"

.PHONY: all clean fclean re