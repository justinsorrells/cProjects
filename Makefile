SRC = maintain.c project6.c 
OBJ = maintain.o project6.o 
PROG = final

$(PROG): $(OBJ)
	gcc $(OBJ) -o $(PROG)

$(OBJ): $(SRC)