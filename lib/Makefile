LIB = biblio.a

SRC = biblio.c

OBJDIR = obj/
OBJ = $(addprefix $(OBJDIR),$(SRC:.c=.o))

all: $(LIB)

$(LIB): $(OBJDIR) $(OBJ)
	ar rc $(LIB) $(OBJ)
	ranlib $(LIB)

# Create an obj/ directory for the *.o files
$(OBJDIR):
	mkdir -p $@

# Compile the *.o files
$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(LIB)

re: fclean all

