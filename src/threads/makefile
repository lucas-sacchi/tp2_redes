CC = gcc
CFLAGS = -Wall -Wextra -g
SRCS = server_thread.c servidor.c 
TARGET = server_thread
all: $(TARGET)
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -lpthread
clean:
	rm -f $(TARGET)
rebuild: clean all
run: $(TARGET)
	./$(TARGET)
