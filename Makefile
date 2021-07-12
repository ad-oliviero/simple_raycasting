CC = gcc
SRC = .
LIBS = $(SRC)/lib/
FILES = $(SRC)/main.c $(SRC)/hud.c $(SRC)/player.c $(SRC)/rendering.c $(SRC)/fighting.c
OBJS = $(SRC)/main.o $(SRC)/hud.o $(SRC)/player.o $(SRC)/rendering.o $(SRC)/fighting.o
CFLAGS = -O3
LDFLAGS = -L $(SRC)/$(LIBS)/raylib -l:libraylib.a -lm -ldl -lm -lpthread -lglfw
WLDFLAGS = -I include/ -L $(SRC)/$(LIBS)/raylib -l:libraylib.a -lopengl32 -lgdi32 -lwinmm -lwinpthread
NAME = "smprc"

build: main hud player rendering fighting
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

build_windows: main hud player rendering fighting
	$(CC) $(CFLAGS) -Wl,--subsystem,windows $(FILES) -o $(NAME) $(WLDFLAGS)

main:
	$(CC) $(CFLAGS) -c $@.c -o $@.o

hud:
	$(CC) $(CFLAGS) -c $@.c -o $@.o

player:
	$(CC) $(CFLAGS) -c $@.c -o $@.o

rendering:
	$(CC) $(CFLAGS) -c $@.c -o $@.o

fighting:
	$(CC) $(CFLAGS) -c $@.c -o $@.o

run:
	@./$(NAME)

clean:
	rm $(NAME) $(OBJS)

debug_build: CFLAGS = -g -Wall -Wextra -D DEBUG
debug_build: build

debug_run: debug_build
	@./$(NAME)

debug_gdb: debug_build
	gdb ./$(NAME)
