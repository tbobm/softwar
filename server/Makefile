NAME	=	server
CC		=	gcc
LIBDIR	=	libs/
INCLUDES=	-Iincludes/
LIBMY	=	$(addprefix $(LIBDIR), libmy.so)
LIBFLAGS=	-L$(LIBDIR) -lmy
CFLAGS	=	-W -Wall -Werror -Wextra $(INCLUDES) $(LIBFLAGS)
SRC_DIR	=	srcs
SRCS	=	$(addprefix srcs/, *.c)
OBJS	=	$(SRCS:.c=.o)
RM		=	rm -rf

.default: $(NAME)

.PHONY: $(LIBMY) $(NAME)

$(LIBMY):
	$(MAKE) -C $(LIBDIR)

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBDIR)

all: $(NAME)

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(NAME): $(LIBMY) $(OBJS)
	$(CC) $(OBJS) $(LIBFLAGS) $(CFLAGS) -o $(NAME)

help:
	@echo "In order to ru nthe program"
	@echo "Run the following command:"
	@echo "LD_LIBRARY_PATH=:$(LIBDIR) ./$(NAME) [args]"
