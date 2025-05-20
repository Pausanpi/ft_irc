NAME = ircserver
INCLUDE = includes
SRC_DIR = src
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

SRC_FILES = Main.cpp Server.cpp Client.cpp Channel.cpp \
			commands/CommandHandler.cpp \
			commands/Join.cpp commands/NickUser.cpp commands/Privmsg.cpp \
			commands/Quit.cpp \

OBJS = $(SRCS:src/%.c=obj/%.o)

###########################################################################

all: $(NAME)

$(NAME): $(OBJS)
			@$(CC) -I./$(INCLUDE) $(CFLAGS) $(OBJS) -o $(NAME) 
			@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"

obj/%.o: src/%.c
	@mkdir -p obj
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

clean:
			@clear
			@rm -rf obj
			@echo "$(BLUE)$(NAME) object files cleaned!$(DEF_COLOR)"

fclean:		
			@clear
			@rm -rf obj
			@rm -f $(NAME)
			@echo "$(BLUE)$(NAME) executable cleaned!$(DEF_COLOR)"

re:			fclean all
			@echo "$(MAGENTA)$(NAME) recompiled!$(DEF_COLOR)"

.PHONY: all clean fclean re