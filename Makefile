NAME = ircserv

FLAG = -Wall -Werror -Wextra -std=c++98 #-fsanitize=address -g

HEADER = server/Authentication.hpp server/Client.hpp server/Server.hpp
EXP_HEADER = exception/FcntlFailedException.hpp exception/SocketBindFailedException.hpp  exception/SocketFailedToCreatException.hpp  exception/SocketListenFailedException.hpp  exception/SocketOptionFailedException.hpp 

EXP_SRC = exception/FcntlFailedException.cpp exception/SocketBindFailedException.cpp exception/SocketFailedToCreatException.cpp exception/SocketListenFailedException.cpp exception/SocketOptionFailedException.cpp \
		commands/channel_membership/channel.cpp \
		commands/channel_membership/join.cpp  \
		commands/channel_membership/Reply.cpp \


CPP = c++

SERVER_SRC = server/Authentication.cpp server/Client.cpp server/Server.cpp main.cpp
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

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
