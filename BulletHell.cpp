#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>

int vertSize = 30, horSize = 20;	//vertical and horizontal dimensions of window
int size = 16;						//number of pixels
int width = size * horSize;			//width of window
int height = size * vertSize;		//height of window

//Player movement variables
int direction = 0;	//when direction = 0, player does not move
int playerHealth = 4;
int laserNum = 20; //Maximum amount of laser generated, will increase over time
float keyDelay = 180;	//Delay between button presses to avoid overreading
float surviveTime = 0;	//Records total time alive, will be used to score player at end
int laserNumDelay = 0;	//Records time before num of enemies increase
int speedDelay = 0;		//Records time before delay decreases
int damageDelay = 0;


//Player
struct Player {
	int x, y;
	bool active = true;
}p[4];

//Laser
struct Laser {
	int x, y;
}l[100];

//player movement
void playerMove() {
	//Only register movement if enough time has passed between key presses
	if (keyDelay > 160) {
		if (direction == 1) {
			if (p[0].x != 0 && p[1].x != 0 && p[2].x != 0 && p[3].x != 0) {
				for (int x = 0; x < 4; x++) {
					if (p[x].active == true) {
						p[x].x -= 1;
					}
				}
			}
		}
		else if (direction == 2) {
			if (p[0].x != 19 && p[1].x != 19 && p[2].x != 19 && p[3].x != 19) {
				for (int x = 0; x < 4; x++) {
					if (p[x].active == true) {
						p[x].x += 1;
					}
				}
			}
		}
		keyDelay = 0;
	}
	direction = 0;	//player only moves one space at a time
}

//enemy movement
void laserMove() {
	for (int x = 0; x < laserNum; x++) {
		l[x].y++;
		if (l[x].y > 29) {
			l[x].x = rand() % 19 + 0;
			l[x].y = 0;
		}
	}
}

int main()
{
	//set pseudorandom time
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(width, height), "Bullet Hell");
	
	//load textures
	sf::Texture t1, t2, t3;
	t1.loadFromFile("Images/background.png");
	t2.loadFromFile("Images/laser.png");
	t3.loadFromFile("Images/spaceship.png");

	//assign sprites
	sf::Sprite background(t1);
	sf::Sprite enemy(t2);
	sf::Sprite player(t3);

	//initialize starting position of player
	p[0].x = 9;
	p[0].y = 29;
	p[1].x = 10;
	p[1].y = 29;
	p[2].x = 9;
	p[2].y = 28;
	p[3].x = 10;
	p[3].y = 28;

	//initialize starting position of initial enemies
	for (int x = 0; x < laserNum; x++) {
		l[x].x = rand() % 19 + 0;
		l[x].y = rand() % 15 + 0;
	}
	
	//setting time
	sf::Clock clock;
	float timer = 0.0f, delay = 0.3f;

	//window opens
	while (window.isOpen()) {
		//setting up timer clock
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		sf::Event e;

		//check if window is closed
		while (window.pollEvent(e)) {
			//close window if x is hit
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}

		//player control
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			direction = 1;
			playerMove();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			direction = 2;
			playerMove();
		}

		//move lasers downwards
		if (timer > delay) {
			timer = 0;
			laserMove();
			//Deal damage on collision with player
			for (int x = 0; x < laserNum; x++) {
				for (int y = 0; y < 4; y++) {
					if (l[x].x == p[y].x && l[x].y == p[y].y) {
						p[y].x = -10;
						p[y].y = -10;
						p[y].active = false;
						playerHealth--;
						damageDelay = 300;
						if (playerHealth == 0) {
							window.close();
						}
						l[x].x = rand() % 19 + 0;
						l[x].y = 0;
					}
				}
			}
		}

		//draw window
		window.clear();

		//creating background
		for (int x = 0; x < horSize; x++) {
			for (int y = 0; y < vertSize; y++) {
				//set position of background sprites
				background.setPosition(x*size, y*size);
				window.draw(background);
			}
		}

		//draw player
		for (int x = 0; x < 4; x++) {
			player.setPosition(p[x].x*size, p[x].y*size);
			window.draw(player);
		}

		//display lasers
		for (int x = 0; x < laserNum; x++) {
			enemy.setPosition(l[x].x*size, l[x].y*size);
			window.draw(enemy);
		}
		
		//increment the delays
		keyDelay++;
		laserNumDelay++;
		speedDelay++;
		if (damageDelay > 0) {
			damageDelay--;
		}
		//increment survive time
		surviveTime++;
		//increase enemy count if time reaches a certain point
		if (laserNumDelay==25000) {
			if (laserNum < 100) {
				laserNum = laserNum + 10;
			}
			laserNumDelay = 0;
		}
		//decrease delay if time reaches a certain point
		if (speedDelay==40000) {
			if (delay > 0.1f) {
				delay = delay - 0.1f;
			}
			speedDelay = 0;
		}
		//display everything
		window.display();
	}

	std::cout << "Game Over!" << std::endl;
	std::cout << "Your score is: " << surviveTime / 1000 << " points" << std::endl;
	return 0;
}
