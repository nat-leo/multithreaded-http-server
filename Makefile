CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -O2 -pthread
TARGET = server

$(TARGET): server.c
	$(CC) $(CFLAGS) server.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean