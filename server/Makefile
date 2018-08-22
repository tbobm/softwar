CC		=	gcc
LIB		=	-lczmq -lpthread
NAME	=	server
SRC 	=	srcs/args.c			\
			srcs/player.c		\
			srcs/energy.c		\
			srcs/server.c		\
			srcs/server_RFC.c	\
			main.c
OBJ		=	$(SRC:%.c=%.o)
CFLAGS	=	-W -Wall -Werror

$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) $(LIB) $(OBJ) -o $(NAME)

all:		$(NAME)

clean:
			$(RM) $(OBJ)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
