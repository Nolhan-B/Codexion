NAME = codexion

CC = cc
CCFLAGS = -Wall -Wextra -Werror -pthread

SRC = \
codexion.c \
main.c \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@ -I .

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re