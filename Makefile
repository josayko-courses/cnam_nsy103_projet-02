# Executable name
EXE = projet2
BIS = projet2bis

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# Source files
SRCDIR = src/
SRC = projet2.c \
			projet2bis.c

# Lib
LIB = lib/biblio.a

# Includes
INC = -Ilib

# *.o files
OBJDIR = obj/
OBJ = $(addprefix $(OBJDIR),$(SRC:.c=.o))

all: $(EXE) $(BIS)

# Make the executable `project2`
$(EXE): $(OBJDIR) $(OBJ)
	make -C lib/ 1>/dev/null
	$(CC) $(addprefix $(OBJDIR),projet2.o) $(INC) $(LIB) -o $(EXE)

# Create an obj/ directory for the *.o files
$(OBJDIR):
	mkdir -p $@

# Compile the *.o files
$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BIS): $(EXE)
	$(CC) $(addprefix $(OBJDIR),projet2bis.o) $(INC) $(LIB) -o $(BIS)

clean:
	rm -rf $(OBJDIR)
	make clean -C lib/ 1>/dev/null

fclean: clean
	rm -rf $(EXE) $(BIS)
	make fclean -C lib/ 1>/dev/null

re: fclean all
