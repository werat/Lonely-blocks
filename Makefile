NAME = SDLGame
BIN = bin
OBJ = obj
SRC = src

# instead of this there should be 'sdl-config --libs' and other
INCLUDE = C:\SDL\include\SDL2
LIB = C:\SDL\lib

CC = g++
CFLAGS = -I$(INCLUDE) -std=gnu++11 -O2 -Wall
LFLAGS = -L$(LIB) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image 
# in Release (for windows):
# LFLAGS += -mwindows

CORE_OBJS = app.o renderer.o SDL_game.o vector2.o rigid_body.o physics_engine.o world.o main.o

OBJS = $(addprefix $(OBJ)/,$(CORE_OBJS))

# top-level rule to create the program.
all: $(NAME)


# linking the program
$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(BIN)\$(NAME).exe $(LFLAGS) 


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
