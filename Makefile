ROOTNAME=main
RUNOPTIONS=50000
CC=gcc
CFLAGS= -g -I.
LIBS =
DEPS = 
OBJ = $(ROOTNAME).o hash_table.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(ROOTNAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)  $(LIBS)

clean:
	rm *.o $(ROOTNAME)

run: $(ROOTNAME)
	./$(ROOTNAME) $(RUNOPTIONS)