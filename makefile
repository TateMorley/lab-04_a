go: go.o calculator.o user.o
	gcc go.o calculator.o user.o -o go

go.o: go.c go.h
	gcc -c go.c -o go.o

calculator.o: calculator.c calculator.h
	gcc -c calculator.c -o calculator.o

user.o: user.c user.h
	gcc -c user.c -o user.o

clean:
	rm -f *.o go
