all:
	g++ -Wall -O0 -g -o app main.cpp -lsfml-system -lsfml-window -lsfml-graphics
run:
	./app
clean:
	rm app