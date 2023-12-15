all:
	g++ -Wall -o app main.cpp -lsfml-system -lsfml-window -lsfml-graphics
run:
	./app
clean:
	rm app