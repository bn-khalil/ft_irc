NAME = ircserv

SRC = main.cpp

HEDERS = Bureaucrat.hpp

OBJ = $(SRC:.cpp=.o)
DEP = $(SRC:.cpp=.d)

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all
