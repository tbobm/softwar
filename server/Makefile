NAME	=	server
CC		=	gcc
LIBDIR	=	libs
INCLUDES=	-Iincludes/
LIBMY	=	$(LIBDIR)/libmy.so
LIBFLAGS=	-L$(LIBDIR) -lmy
CFLAGS	=	-W -Wall -Werror -Wextra $(INCLUDES) $(LIBFLAGS)
SRC_DIR	=	srcs
SRCS	=	$(wildcard srcs/*.c)
OBJS	=	$(SRCS:.c=.o)
RM		=	rm -rf

.default: $(NAME)

.PHONY: $(LIBMY)

$(LIBMY):
	$(MAKE) -C $(LIBDIR)

clean:
	$(RM) $(OBJS)

all: $(NAME)

$(NAME): $(OBJS) $(LIBMY)
	$(CC) $(OBJS) $(LIBFLAGS) $(CFLAGS) -o $(NAME)

help:
	@echo "In order to ru nthe program"
	@echo "Run the following command:"
	@echo "LD_LIBRARY_PATH=:$(LIBDIR) ./$(NAME) [args]"
