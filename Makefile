CXX=clang++
CPPFLAGS=-Wall -std=c++2a

all:	main.o
	clang++ -Wall -std=c++2a -o main main.o

clean:
	rm -f main
	rm -f main.o

run:	all
	./main