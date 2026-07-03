#include <SFML\Graphics.hpp>

class Score
{
private:
	int score;
	sf::Text text;
public:
	Score(sf::Font& font, float posX, float posY): text(font)
	{
		this->score = 0;
		text.setPosition({ posX, posY });
		text.setCharacterSize(50);
		text.setFillColor(sf::Color::White);
		updateText();

	}

	void increment()
	{
		score++;
		updateText();
	}

	void updateText()
	{
		text.setString(std::to_string(score));
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(text);
	}
};

//==============================================================================================================================================
//PADDLE CLASS
//==============================================================================================================================================
class Paddle
{
private:
	sf::RectangleShape shape;
	float speed;
	int WIDTH, HEIGHT;
	float paddleWidth, paddleHeight;
	sf::Keyboard::Key upKey;
	sf::Keyboard::Key downKey;
public:
	Paddle(int WIDTH, int HEIGHT, float paddleWidth, float paddleHeight, float speed, sf::Color color, float posX, float posY, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey)
	{
		this->WIDTH = WIDTH;
		this->HEIGHT = HEIGHT;
		this->speed = speed;
		this->paddleWidth = paddleWidth;
		this->paddleHeight = paddleHeight;
		this->upKey = upKey;
		this->downKey = downKey;
		shape.setSize({ paddleWidth, paddleHeight });
		shape.setPosition({ posX, posY });
		shape.setFillColor(color);

	}
	void move(float dt)
	{
		if (sf::Keyboard::isKeyPressed(upKey) && shape.getPosition().y >= 0)
		{
			shape.move({ 0, -speed * dt });
		}
		if (sf::Keyboard::isKeyPressed(downKey) && shape.getPosition().y <= HEIGHT - paddleHeight)
		{
			shape.move({ 0, speed * dt });
		}
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(shape);
	}

	sf::FloatRect getBounds() const
	{
		return shape.getGlobalBounds();
	}
};

//==============================================================================================================================================
//BALL CLASS
//==============================================================================================================================================
class Ball
{
private:
	sf::CircleShape shape;
	float speedX, speedY;
	int WIDTH, HEIGHT;
	float radius, diameter;

public:
	Ball(float speedX, float speedY, float radius, sf::Color color, int WIDTH, int HEIGHT)
	{
		this->speedX = speedX;
		this->speedY = speedY;
		this->WIDTH = WIDTH;
		this->HEIGHT = HEIGHT;
		this->radius = radius;
		diameter = radius * 2.f;
		shape.setRadius(radius);
		shape.setFillColor(color);

		reset();

	}

	void move(float dt)
	{
		shape.move({ speedX * dt, speedY * dt });
		bounceY();
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(shape);
	}

	void bounceY()
	{
		if (shape.getPosition().y <= 0 || shape.getPosition().y >= HEIGHT - diameter)
		{
			speedY = -speedY;
		}
	}

	void reset()
	{
		shape.setPosition({ WIDTH / 2 - radius, HEIGHT / 2 - radius });
	}

	void paddleCollision(const Paddle& paddle)
	{
		if (shape.getGlobalBounds().findIntersection(paddle.getBounds()))
		{
			speedX = -speedX;
		}
	}

	bool isOutLeft()
	{
		return shape.getPosition().x < -diameter;
	}

	bool isOutRight()
	{
		return shape.getPosition().x > WIDTH;
	}
};


//==============================================================================================================================================
//MAIN FUNCTION
//==============================================================================================================================================
int main()
{
	const int WIDTH = 1280, HEIGHT = 720;
	sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "Ping-Pong Game");
	sf::Font font("LowresPixel-Regular.otf");
	sf::Clock clock;

	Ball pingPong(300.f, 200.f, 20.f, sf::Color::Yellow, WIDTH, HEIGHT);

	Paddle leftPaddle(WIDTH, HEIGHT, 20, 100, 300.f, sf::Color::Red, 50, HEIGHT/2 - 50, sf::Keyboard::Key::W, sf::Keyboard::Key::S);
	Paddle rightPaddle(WIDTH, HEIGHT, 20, 100, 300.f, sf::Color::Blue, WIDTH - 70, HEIGHT/2 - 50, sf::Keyboard::Key::Up, sf::Keyboard::Key::Down);
	
	Score leftScore(font, 320.f, 20.f);
	Score rightScore(font, 960.f, 20.f);

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}
		float dt = clock.restart().asSeconds();
		window.clear(sf::Color::Black);

		pingPong.move(dt);
		pingPong.draw(window);
		pingPong.paddleCollision(leftPaddle);
		pingPong.paddleCollision(rightPaddle);

		if (pingPong.isOutLeft())
		{
			rightScore.increment();
			pingPong.reset();
		}

		if (pingPong.isOutRight())
		{
			leftScore.increment();
			pingPong.reset();
		}

		leftPaddle.move(dt);
		rightPaddle.move(dt);

		leftPaddle.draw(window);
		rightPaddle.draw(window);

		leftScore.draw(window);
		rightScore.draw(window);

		window.display();
	}
	return 0;
}