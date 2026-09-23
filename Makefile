CC = gcc
CFLAGS = -Wall -Iinclude

SRCS = src/main.c src/lexer.c src/parser.c
OBJS = $(SRCS:.c=.o)
TARGET = micro-pascal

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
