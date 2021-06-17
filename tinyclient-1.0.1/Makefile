CC=g++
CCFLAGS=-g -Wall

O_FILES = tinyclient.o

startup : $(O_FILES)
	$(CC) $(CCFLAGS) -o tinyclient $(O_FILES)

.SUFFIXES : .o .c

.c.o :  
	$(CC) $(CCFLAGS) -c $<

clean:
	rm *.o
