CC = gcc
CFLAGS = -Wall -Wextra -g
SRCS = server_iterativo.c servidor.c
TARGET = server_iterativo
all: $(TARGET)
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
clean:
	rm -f $(TARGET)
rebuild: clean all
run: $(TARGET)
	./$(TARGET)
