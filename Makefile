NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -I .

SRCS = codexion.c main.c init.c utils.c destroy_sim.c init_th.c \
       init_utils.c coders.c coder_routine.c monitor.c monitor_utils.c \
       queue_init.c queue_ops.c queue.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re