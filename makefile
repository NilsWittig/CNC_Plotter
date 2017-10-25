all: main.o rs232.o
	gcc -o main main.o rs232.o

main.o: main.c rs232.h
	gcc -Wall -Wextra -Wshadow -Wformat-nonliteral -Wformat-security -Wtype-limits -o2 -c main.c -o main.o

rs232.o: rs232.c rs232.h
	gcc -Wall -Wextra -Wshadow -Wformat-nonliteral -Wformat-security -Wtype-limits -o2 -c rs232.c -o rs232.o