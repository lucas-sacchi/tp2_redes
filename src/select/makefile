CC = gcc
CFLAGS = -Wall -Wextra -g
SRCS = server_select.c servidor.c
TARGET = server_select
all: $(TARGET)
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
clean:
	rm -f $(TARGET)
rebuild: clean all
run: $(TARGET)
	./$(TARGET)
