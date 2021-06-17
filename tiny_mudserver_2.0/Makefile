CC=g++
#CCFLAGS=-g3 -Wall -w -pedantic
CCFLAGS=-g3 -Wall  #YKL

O_FILES = tinymudserver.o

tinymudserver : $(O_FILES)
	$(CC) $(CCFLAGS) -o tinymudserver $(O_FILES)

.SUFFIXES : .o .cpp

.cpp.o :  
	$(CC) $(CCFLAGS) -c $<

clean:
	rm *.o
