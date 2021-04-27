CC = gcc
SRC = .
FILES = $(SRC)/main.c $(SRC)/hud.c $(SRC)/player.c $(SRC)/rendering.c
CFLAGS = -O3 -Wall -Wextra
LDFLAGS = -lm -lpthread -lglfw -L $(SRC)/raylib -l:libraylib.a -lpthread -lm -ldl
NAME = "main"

build:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME) $(LDFLAGS)

run:
	@./$(NAME)

clean:
	@rm -f $(NAME)

debug: build run clean
