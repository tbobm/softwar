NAME	=	server
CC		=	gcc
CFLAGS	=	-W -Wall -Werror -Wextra
LIBDIR	=	libs/
INCLUDES=	includes/
LIBMY	=	$(LIBDIR)/libmy.so
LIBFLAGS=	-L$(LIBDIR) -lmy
SRC_DIR	=	srcs
SRCS	=	$(SRC_DIR)/main.c

$(LIBMY):
	$(MAKE) -C $(LIBDIR)

all:
	$(CC) $(SRCS) $(LIBFLAGS) -I$(INCLUDES) -o $(NAME)


help:
	@echo "In order to ru nthe program"
	@echo "Run the following command:"
	@echo "LD_LIBRARY_PATH=:$(LIBDIR) ./$(NAME) [args]"
