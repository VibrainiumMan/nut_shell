CC = gcc
CFLAGS = -Wall -Werror -Wextra -lreadline

SRC = src

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(SRCS:$(SRC)/*.c=$(OBJ)/*.o)

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(SRC)/*.o 
run:
	./main

.PHONY: all run clean 
