# define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# define libraries to be linked (for example -lm)
LIB = 

# define sets of header source files and object files
SRC = driver.c data.c linked_list.c

# OBJ is the same as SRC, just replace .c with .h
OBJ = $(SRC:.c=.o)
 
# define the executable name(s)
EXE1 = dict1
EXE2 = dict2

# the first target:
$(EXE1): $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE1) $(OBJ) $(LIB)

$(EXE2): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ) $(LIB)

driver.o: driver.c data.h linked_list.h

data.o: data.c data.h

linked_list.o: linked_list.c linked_list.h data.h

clean:
	rm -f $(OBJ) $(OBJ) $(EXE1) $(EXE2)
