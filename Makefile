CC = gcc
FILES = main.c hud.c player.c rendering.c
CFLAGS = -O3 -Wall -Wextra
LDFLAGS = -lraylib -lm -lpthread
NAME = "main"

build:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME) $(LDFLAGS)

run:
	@./$(NAME)

clean:
	@rm -f $(NAME)

debug: build run clean
