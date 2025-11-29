NAME = ircserv

FLAG = -Wall -Werror -Wextra -std=c++98 -fsanitize=address

HEADER = server/Client.hpp server/Server.hpp  commands/channel.hpp commands/Reply.hpp 

EXP_SRC = commands/channel.cpp \
		commands/join.cpp \
		commands/topic.cpp \
		commands/mode.cpp \
		commands/Reply.cpp \
		commands/kick.cpp \
		commands/invite.cpp \
		commands/privmsg.cpp \
		commands/quite.cpp

CPP = c++

SERVER_SRC = server/Client.cpp server/Server.cpp main.cpp

SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CMD_OBJ = $(CMD_SRC:.cpp=.o)
EXP_OBJ = $(EXP_SRC:.cpp=.o)

OBJ = $(SERVER_OBJ) $(CMD_OBJ) $(EXP_OBJ)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(FLAG) $(OBJ) -o $(NAME)

%.o: %.cpp  $(HEADER)
	$(CPP) $(FLAG) -c $< -o $@


SRC_BONUS = bot.cpp
OBJ_BONUS = $(SRC_BONUS:.cpp=.o)

bonus: $(OBJ_BONUS) Bot.hpp
	$(CPP) $(FLAG) $(OBJ_BONUS) -o bonus


clean:
	rm -rf $(OBJ) $(OBJ_BONUS)

fclean: clean
	rm -rf $(NAME) bonus

re: fclean all
