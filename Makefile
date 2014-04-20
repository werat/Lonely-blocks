NAME = lonely_blocks
BIN = bin
OBJ = obj
SRC = src

CC = g++
CFLAGS = `sdl2-config --cflags` -std=gnu++11 -O2 -Wall
LFLAGS = `sdl2-config --libs`

CORE_OBJS = app.o SDL_game.o vector2.o renderer.o rigid_body.o physics_engine.o scene.o main.o
CORE_OBJS += game_object.o component.o physics_component.o block_renderer.o

OBJS = $(addprefix $(OBJ)/,$(CORE_OBJS))

# top-level rule to create the program.
all: $(NAME)

# linking the program
$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(BIN)/$(NAME) $(LFLAGS)


# compiling other source files.
$(OBJ)/%.o: $(SRC)/%.cc
	$(CC) $(CFLAGS) -MMD -c -s $< -o $@

-include $(OBJ)/*.d

$(OBJS): | $(OBJ)
$(NAME): | $(BIN)

$(OBJ):
	mkdir $(OBJ)
$(BIN):
	mkdir $(BIN)

clean:
	rm -rf $(OBJ)
