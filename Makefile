NAME = ircserv

FLAG = -Wall -Werror -Wextra #-std=c++98 -fsanitize=address -g

HEADER = server/Client.hpp server/Server.hpp

EXP_SRC =commands/channel_membership/channel.cpp \
		commands/channel_membership/join.cpp  \
		commands/channel_membership/topic.cpp  \
		commands/channel_membership/mode.cpp  \
		commands/channel_membership/Reply.cpp \
		commands/channel_membership/kick.cpp \
		commands/channel_membership/invite.cpp \
		commands/channel_membership/privmsg.cpp \

CPP = c++

SERVER_SRC = server/Client.cpp server/Server.cpp main.cpp
CMD_SRC =

SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CMD_OBJ = $(CMD_SRC:.cpp=.o)
EXP_OBJ = $(EXP_SRC:.cpp=.o)

OBJ = $(SERVER_OBJ) $(CMD_OBJ) $(EXP_OBJ)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(FLAG) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CPP) $(FLAG) -c $< -o $@


SRC_BONUS = bot.cpp
OBJ_BONUS = $(SRC_BONUS:.cpp=.o)

bonus: $(OBJ_BONUS)
	$(CPP) $(FLAG) $(OBJ_BONUS) -o bonus

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

