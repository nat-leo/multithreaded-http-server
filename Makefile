CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -O2 -pthread
TARGET = server http

SRCS := server.c http.c

OBJS := $(SRCS:.c=.o ) 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean