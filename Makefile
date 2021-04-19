#---------------------------------------------------
# Target file to be compiled by default
#---------------------------------------------------
MAIN = main
#---------------------------------------------------
# CC is the compiler to be used
#---------------------------------------------------
CC = gcc
#---------------------------------------------------
# CFLAGS are the options passed to the compiler
#---------------------------------------------------
CFLAGS = -Wall -lpthread -lrt -lm
# CFLAGS = -Wall -lpthread -lrt -lm -ggdb
DEBUGFLAGS = -g -ggdb
#---------------------------------------------------
# OBJS are the object files to be linked
#---------------------------------------------------
OBJ1 = Avversario
OBJ2 = Pallina
OBJ3 = PTask
OBJ4 = Robot_camera
OBJ5 = Task_grafico
OBJ6 = taskmotor
OBJS = $(MAIN).o $(OBJ1).o $(OBJ2).o $(OBJ3).o $(OBJ4).o $(OBJ5).o $(OBJ6).o
#---------------------------------------------------
# LIBS are the external libraries to be used
#---------------------------------------------------
LIBS = `allegro-config --libs`
#---------------------------------------------------
# Dependencies
#---------------------------------------------------
$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LIBS) $(CFLAGS) $(DEBUGFLAGS)
$(MAIN).o: $(MAIN).c
	$(CC) -c $(MAIN).c $(DEBUGFLAGS)
$(OBJ1).o: $(OBJ1).c
	$(CC) -c $(OBJ1).c $(DEBUGFLAGS)
$(OBJ2).o: $(OBJ2).c
	$(CC) -c $(OBJ2).c $(DEBUGFLAGS)
$(OBJ3).o: $(OBJ3).c
	$(CC) -c $(OBJ3).c $(DEBUGFLAGS)
$(OBJ4).o: $(OBJ4).c
	$(CC) -c $(OBJ4).c $(DEBUGFLAGS)
$(OBJ5).o: $(OBJ5).c
	$(CC) -c $(OBJ5).c $(DEBUGFLAGS)
$(OBJ6).o: $(OBJ6).c
	$(CC) -c $(OBJ6).c $(DEBUGFLAGS)
#---------------------------------------------------
# Command that can be specified inline: make run
#---------------------------------------------------
run: $(MAIN)
	sudo ./$(MAIN)
#---------------------------------------------------
# Command that can be specified inline: make clean
#---------------------------------------------------
clean:
	rm -rf *.o $(MAIN)
#---------------------------------------------------
# Command that can be specified inline: make rundebug
#---------------------------------------------------
rundebug: $(MAIN)
	sudo gdb main