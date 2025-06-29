CC = gcc
CFLAGS = -Wall -Wextra
SRC = main.c socket/socket.c socket/conn.c utils/logger.c utils/parser.c proxy/proxy.c utils/helper.c
OBJ = $(SRC:.c=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o socket/*.o utils/*.o proxy/*.o $(TARGET)

.PHONY: all clean
