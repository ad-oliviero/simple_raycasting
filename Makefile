CC = gcc
SRC = .
FILES = $(SRC)/main.c $(SRC)/hud.c $(SRC)/player.c $(SRC)/rendering.c
CFLAGS = -O3
DBFLAGS = -g -Wall -Wextra
LDFLAGS = -L $(SRC)/raylib -l:libraylib.a -lm -ldl -lm -lpthread -lglfw
WLDFLAGS = -I include/ -L $(SRC)/raylib -l:libraylib.a -lopengl32 -lgdi32 -lwinmm -lwinpthread
NAME = "main"

build:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME) $(LDFLAGS)

build_windows:
	$(CC) $(CFLAGS) -Wl,--subsystem,windows $(FILES) -o $(NAME) $(WLDFLAGS)

run:
	@./$(NAME)

clean:
	@rm -f $(NAME)

debug_build:
	$(CC) $(DBFLAGS) $(FILES) -o $(NAME) $(LDFLAGS)

debug_run: debug_build
	@./$(NAME)
	make clean
