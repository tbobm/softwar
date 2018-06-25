CC = gcc
NAME = libmy
INCLUDES = ../includes/
CFLAGS = -fPIC -Wall -Wextra -O2 -I$(INCLUDES)
LDFLAGS = -shared
RM = rm -f
TARGET_LIB = $(NAME).so

SRCS = log.c
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)


