#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdio>
#include <ctime>

enum Orientation { vertical, horizontal };
enum Directions { UP, RIGHT, DOWN, LEFT };

class Line
{
private:
	sf::RectangleShape line;
public:
	Line() : line() {};
	Line(float length, bool orientation)
	{
		if (orientation == vertical)
			line.setSize(sf::Vector2f(1.f, length));
		if (orientation == horizontal)
			line.setSize(sf::Vector2f(length, 1.f));
	}
	void set_length(float length, bool orientation)
	{
		if (orientation == vertical)
			line.setSize(sf::Vector2f(1.f, length));
		if (orientation == horizontal)
			line.setSize(sf::Vector2f(length, 1.f));
	}
	void set_position(sf::Vector2f& position)
	{
		line.setPosition(position);
	}
	void draw(sf::RenderWindow& window)
	{
		window.draw(line);
	}
	void set_color(sf::Color& color)
	{
		line.setFillColor(color);
	}
};

class Field
{
private:
	std::vector<Line> lines;
	float distance;
	float pixel_size;
	int block_size;
	sf::Vector2f position;
public:
	Field(float set_pixel_size, int set_block_size, float x, float y) :
		pixel_size(set_pixel_size),
		block_size(set_block_size),
		distance(set_pixel_size / set_block_size),
		position(sf::Vector2f(x, y))
	{
		sf::Vector2f position(x, y);
		int i;
		for (i = 0; i < block_size + 1; i++)
		{
			lines.push_back(Line(pixel_size, vertical));
			lines[i].set_position(position);
			position.x += distance;
		}
		position = sf::Vector2f(x, y);
		for (int i = block_size + 1; i < block_size * 2 + 2; i++)
		{
			lines.push_back(Line(pixel_size, horizontal));
			lines[i].set_position(position);
			position.y += distance;
		}
	}
	void draw(sf::RenderWindow& window)
	{
		for (auto& line : lines)
			line.draw(window);
	}
	void set_color(sf::Color color)
	{
		for (auto& line : lines)
			line.set_color(color);
	}
	float get_pixel_size()
	{
		return pixel_size;
	}
	int get_block_size()
	{
		return block_size;
	}
	sf::Vector2f get_position()
	{
		return position;
	}
};

class Block
{
private:
	sf::RectangleShape block;
	float size;
	int direction;
public:
	Block() : block() {};
	Block(float size, sf::Vector2f position) : size(size), block(sf::Vector2f(size, size)), direction(0)
	{
		block.setPosition(position);
	}
	sf::Vector2f get_position()
	{
		return block.getPosition();
	}
	void set_position(sf::Vector2f new_position)
	{
		block.setPosition(new_position);
	}
	void set_direction(int new_direction)
	{
		if (new_direction > LEFT)
			return;
		direction = new_direction;
	}
	int get_direction()
	{
		return direction;
	}
	void move()
	{
		if (direction == UP)
			block.move(sf::Vector2f(0, -size));
		if (direction == RIGHT)
			block.move(sf::Vector2f(size, 0));
		if (direction == DOWN)
			block.move(sf::Vector2f(0, size));
		if (direction == LEFT)
			block.move(sf::Vector2f(-size, 0));
	}
	void draw(sf::RenderWindow& window)
	{
		window.draw(block);
	}
	void set_color(sf::Color color)
	{
		block.setFillColor(color);
	}
};

class Snake
{
private:
	std::vector<Block> snake;
	Block* food;
	std::vector<sf::Vector2f> turns;
	std::vector<int> turn_direction;
	float field_pixel_size;
	int field_block_size;
	sf::Vector2f field_position;
	float block_size;
	float up_border, right_border, down_border, left_border;
public:
	Snake(float field_pixel_size, int field_block_size, sf::Vector2f field_position) :
		field_pixel_size(field_pixel_size),
		field_block_size(field_block_size),
		field_position(field_position)
	{
		block_size = field_pixel_size / field_block_size;
		up_border = field_position.y;
		right_border = field_position.x + field_pixel_size;
		down_border = field_position.y + field_pixel_size;
		left_border = field_position.x;
		sf::Vector2f start_position = field_position;
		start_position.x = start_position.x + block_size * (rand() % (field_block_size - 1));
		start_position.y = start_position.y + block_size * (rand() % (field_block_size - 1));
		snake.push_back(Block(block_size, start_position));
		if (field_pixel_size - (start_position.x - field_position.x) < block_size * 3) {
			snake.push_back(Block(block_size, sf::Vector2f(start_position.x - block_size, start_position.y)));
			snake.push_back(Block(block_size, sf::Vector2f(start_position.x - 2 * block_size, start_position.y)));
			snake[0].set_direction(RIGHT);
			snake[1].set_direction(RIGHT);
			snake[2].set_direction(RIGHT);
		}
		else {
			snake.push_back(Block(block_size, sf::Vector2f(start_position.x + block_size, start_position.y)));
			snake.push_back(Block(block_size, sf::Vector2f(start_position.x + 2 * block_size, start_position.y)));
			snake[0].set_direction(LEFT);
			snake[1].set_direction(LEFT);
			snake[2].set_direction(LEFT);
		}
		start_position = field_position;
		start_position.x = start_position.x + block_size * (rand() % (field_block_size - 1));
		start_position.y = start_position.y + block_size * (rand() % (field_block_size - 1));
		food = new Block(block_size, start_position);
		food->set_color(sf::Color::Red);
	}
	void move()
	{
		for (auto& block : snake)
			block.move();
	}
	void draw(sf::RenderWindow& window)
	{
		food->draw(window);
		for (auto& block : snake)
			block.draw(window);
	}
	void change_direction(int new_direction)
	{
		int old_direction = snake[0].get_direction();
		if (new_direction + 2 == old_direction || new_direction - 2 == old_direction)
			return;
		snake[0].set_direction(new_direction);
		turns.push_back(snake[0].get_position());
		turn_direction.push_back(new_direction);
	}
	void check_directions()
	{
		if (turns.empty())
			return;
		for (int i = 1; i < snake.size(); i++)
		{
			for (int j = 0; j < turns.size(); j++)
			{
				if (snake[i].get_position() == turns[j])
					snake[i].set_direction(turn_direction[j]);
			}

		}
		if (snake[snake.size() - 1].get_position() == turns[0]) {
			turns.erase(turns.cbegin());
			turn_direction.erase(turn_direction.cbegin());
		}
	}
	bool check_border_collisions()
	{
		if (snake[0].get_position().y < up_border)
			return 1;
		if (snake[0].get_position().x == right_border)
			return 1;
		if (snake[0].get_position().y == down_border)
			return 1;
		if (snake[0].get_position().x < left_border)
			return 1;
		return 0;
	}
	bool check_food_collisions()
	{
		for (auto& block : snake)
			if (block.get_position() == food->get_position())
				return 1;
		return 0;
	}
	void update_food()
	{
		sf::Vector2f new_food_position = field_position;
		new_food_position.x = new_food_position.x + block_size * (rand() % (field_block_size - 1));
		new_food_position.y = new_food_position.y + block_size * (rand() % (field_block_size - 1));
		food->set_position(new_food_position);
	}
	void grow()
	{
		Block* tail = &snake[snake.size() - 1];
		sf::Vector2f tail_position = tail->get_position();
		switch (tail->get_direction()) {
		case UP:
			snake.push_back(Block(block_size, sf::Vector2f(tail_position.x, tail_position.y + block_size)));
			snake[snake.size() - 1].set_direction(UP);
			break;
		case RIGHT:
			snake.push_back(Block(block_size, sf::Vector2f(tail_position.x - block_size, tail_position.y)));
			snake[snake.size() - 1].set_direction(RIGHT);
			break;
		case DOWN:
			snake.push_back(Block(block_size, sf::Vector2f(tail_position.x, tail_position.y - block_size)));
			snake[snake.size() - 1].set_direction(DOWN);
			break;
		case LEFT:
			snake.push_back(Block(block_size, sf::Vector2f(tail_position.x + block_size, tail_position.y)));
			snake[snake.size() - 1].set_direction(LEFT);
			break;
		}
	}
	bool check_snake_collisions()
	{
		for (int i = 1; i < snake.size(); i++)
			if (snake[0].get_position() == snake[i].get_position())
				return 1;
		return 0;
	}
};

int main() {
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(600, 600), "Snake game");
	window.setSize(sf::Vector2u(600, 600));
	Field gamefield(400, 20, 100, 100);
	gamefield.set_color(sf::Color(255, 255, 255, 150));

	Snake* snake = new Snake(400, 20, sf::Vector2f(100, 100));

	sf::Clock clock;

	clock.restart();

	bool food_collisions = 0;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up)
					snake->change_direction(UP);
				if (event.key.code == sf::Keyboard::Right)
					snake->change_direction(RIGHT);
				if (event.key.code == sf::Keyboard::Down)
					snake->change_direction(DOWN);
				if (event.key.code == sf::Keyboard::Left)
					snake->change_direction(LEFT);
			}
		}
		snake->check_directions();
		food_collisions = snake->check_food_collisions();

		window.clear(sf::Color(100, 100, 100, 255));

		snake->draw(window);
		if (clock.getElapsedTime().asMilliseconds() > 100)
		{
			clock.restart();
			snake->move();
			if (food_collisions) {
				snake->grow();
				snake->update_food();
				food_collisions = 0;
			}
			if (snake->check_snake_collisions())
				snake = new Snake(400, 20, sf::Vector2f(100, 100));
		}
		gamefield.draw(window);
		window.display();
		if (snake->check_border_collisions())
			snake = new Snake(400, 20, sf::Vector2f(100, 100));
	}

	return 0;
}
