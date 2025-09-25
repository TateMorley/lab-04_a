# makefile
all: go user calculator

go: go.c
	gcc go.c -o go

user: user.c
	gcc user.c -o user

calculator: calculator.c
	gcc calculator.c -o calculator

clean:
	rm -f go user calculator