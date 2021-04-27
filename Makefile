CC = gcc
SRC = .
FILES = $(SRC)/main.c $(SRC)/hud.c $(SRC)/player.c $(SRC)/rendering.c
CFLAGS = -O3 -Wall -Wextra
LDFLAGS = -lm -lpthread -lglfw -L $(SRC)/raylib -l:libraylib.a -lm -ldl
WLDFLAGS = -I include/ -L $(SRC)/raylib -l:libraylib.a -lopengl32 -lgdi32 -lwinmm
NAME = "main"

build:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME) $(LDFLAGS)

build_windows:
	$(CC) $(CFLAGS) -Wl,--subsystem,windows $(FILES) -o $(NAME) $(WLDFLAGS)
run:
	@./$(NAME)

clean:
	@rm -f $(NAME)

debug: build run clean
