lexica : main.o
	g++ -o lexica main.o

main.o : main.cpp 
	g++ -c main.cpp

clean : 
	rm *.o