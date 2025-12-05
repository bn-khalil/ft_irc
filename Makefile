NAME = ircserv

FLAG = -Wall -Werror -Wextra -std=c++98

SRC = commands/channel.cpp \
        commands/join.cpp \
        commands/topic.cpp \
        commands/mode.cpp \
        commands/Reply.cpp \
        commands/kick.cpp \
        commands/invite.cpp \
        commands/privmsg.cpp \
        commands/quite.cpp \
        server/Client.cpp \
        server/Server.cpp\
        main.cpp
CPP = c++


OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(FLAG) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CPP) $(FLAG) -MMD -c $< -o $@


SRC_BONUS = bot_Bonus/Bot.cpp
OBJ_BONUS = $(SRC_BONUS:.cpp=.o)

-include $(DEP)
bonus: $(OBJ_BONUS) bot_Bonus/Bot.hpp
	$(CPP) $(FLAG) $(OBJ_BONUS) -o bonus

$(OBJ_BONUS): $(SRC_BONUS)
	$(CPP) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ) $(DEP) $(OBJ_BONUS) $(DEP)

fclean: clean
	rm -rf $(NAME) $(DEP) bonus

re: fclean all