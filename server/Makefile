NAME	=	server
CC		=	gcc
CFLAGS	=	-W -Wall -Werror -Wextra
LIBDIR	=	libs/
INCLUDES=	includes/
LIBMY	=	my
SRC_DIR	=	srcs
SRCS	=	$(SRC_DIR)/main.c

all:
	$(CC) $(SRCS) -L$(LIBDIR) -I$(INCLUDES) -l$(LIBMY) -o $(NAME)


help:
	@echo "In order to ru nthe program"
	@echo "Run the following command:"
	@echo "LD_LIBRARY_PATH=:$(LIBDIR) ./$(NAME) [args]"
