LINKS= -lsfml-graphics -lsfml-window -lsfml-system

triaflux: main.o
	g++ $? $(LINKS) -o $@ 

main.o: main.cpp
	g++ -c $? -o $@

