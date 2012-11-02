hw6: hw6.o aes.o
	g++ -Wall -g aes.o hw6.o -o hw6
aes.o: aes.cpp aes.h
	g++ -Wall -g -c aes.cpp
hw6.o: hw6.cpp
	g++ -Wall -g -c hw6.cpp
clean:
	rm -rf *.o hw6
