#include <SFML/Graphics.hpp>

using namespace sf;


float offsetX = 0, offsetY = 0;


const int H = 12;
const int W = 40;


String TileMap[H] = {

"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B         0000                BBBB     B",
"B                                B     B",
"BBB                              B     B",
"B              BB                BB    B",
"B              BB                      B",
"B    B         BB         BB           B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

};

class PLAYER {

public:

	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;

	PLAYER(Texture& image)
	{
		sprite.setTexture(image);
		rect = FloatRect(1 * 75, 1 * 75, 84, 150);

		dx = dy = 0.1;
		currentFrame = 0;
	}


	void update(float time)
	{

		rect.left += dx * time;
		Collision(0);

		if (!onGround) dy = dy + 0.0001 * time;
		rect.top += dy * time;
		onGround = false;
		Collision(1);


		currentFrame += 0.001 * time;
		if (currentFrame > 2) currentFrame -= 2;

		if (dx > 0) sprite.setTextureRect(IntRect(84 * int(currentFrame), 0, 84, 150));
		if (dx < 0) sprite.setTextureRect(IntRect(84 * int(currentFrame) + 84, 0, -84, 150));


		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
	}



	void Collision(int dir)
	{
		for (int i = rect.top / 75; i < (rect.top + rect.height) / 75; i++)
			for (int j = rect.left / 75; j < (rect.left + rect.width) / 75; j++)
			{
				if (TileMap[i][j] == 'B')
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 75 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 75 + 75;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 75 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 75 + 75;   dy = 0; }
				}

				if (TileMap[i][j] == '0')
				{
					TileMap[i][j] = ' ';
				}

			}

	}
};

int main()
{
	RenderWindow window(VideoMode(1440, 1024), "Test!");

	Texture t;
	t.loadFromFile("spritelist.png");

	float currentFrame = 0;

	PLAYER p(t);

	Clock clock;

	RectangleShape rectangle(Vector2f(75, 75));

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 300;

		if (time > 20) time = 20;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			p.dx = -0.1;

		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p.dx = 0.1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (p.onGround) { p.dy = -0.35; p.onGround = false; }
		}

		p.update(time);

		if (p.rect.left > 720) offsetX = p.rect.left - 720;
		if (p.rect.left < 720) offsetX = p.rect.left - 720;
		offsetY = p.rect.top - 512;

		window.clear(Color::White);


		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
			{
				if (TileMap[i][j] == 'B') rectangle.setFillColor(Color::Black);

				if (TileMap[i][j] == '0')  rectangle.setFillColor(Color::Green);

				if (TileMap[i][j] == ' ') continue;

				rectangle.setPosition(j * 75 - offsetX, i * 75 - offsetY);
				window.draw(rectangle);
			}

		window.draw(p.sprite);
		window.display();
	}

	return 0;
}