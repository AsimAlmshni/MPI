CC=mpic++
CFLAGS= -I -O3 -Wall

DEPS=
OBJ=main.o genkeys.o


all:  poddeven genkeys

integrate:main.o genkeys
	g++ -o $@ $^

main: main.o genkeys.o 
	mpic++  -o $@ $^

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -O3 -o $@ $^

test1:	main
	mpirun -np 1 ./main 100

test2:	main
	mpirun -np 4 ./main 100

test3:	main
	mpirun -np 7 ./main 100

clean:
	rm -f *.o *~core

