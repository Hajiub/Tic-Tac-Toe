CC = cc 
SRC = main.c 
FLAGS = -Wall -Wextra
EXEC = out
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all:
	$(CC) $(FLAGS) $(SRC) -o $(EXEC) $(LIBS)

clean:
	rm -f $(EXEC)

run:
	./$(EXEC)
