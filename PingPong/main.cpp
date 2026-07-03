#include <SFML/Graphics.hpp>

void paddleControl(sf::RectangleShape& rightPaddle, sf::RectangleShape& leftPaddle, float paddleSpeed, float dt, int HEIGHT, float paddleHeight);
void ballMovement(sf::CircleShape& ball, float ballSpeedX, float& ballSpeedY, float dt, float HEIGHT, float ballDiameter);
void paddleCollision(sf::RectangleShape& leftPaddle, sf::RectangleShape& rightPaddle, sf::CircleShape& ball, float& ballSpeedX);
void playerMiss(sf::CircleShape& ball, int& leftPlayerScore, int& rightPlayerScore, float ballRadius, float ballDiameter, int WIDTH, int HEIGHT, float& ballSpeedY);
void updateScoreText(sf::Text& leftPlayerScoreText, sf::Text& rightPlayerScoreText, int leftPlayerScore, int rightPlayerScore);
void draw(sf::RenderWindow& window, sf::RectangleShape& leftPaddle, sf::RectangleShape& rightPaddle, sf::CircleShape& ball, sf::Text& leftPlayerScoreText, sf::Text& rightPlayerScoreText);



int main()
{
	const int WIDTH = 1280, HEIGHT = 720;

	sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Ping-Pong Game");
	sf::Clock clock;
	sf::Font font("LowresPixel-Regular.otf");

	//--------------------------------------------------------------------------------------------------
	//PLAYER SCORES TEXT SETTINGS
	//--------------------------------------------------------------------------------------------------
	sf::Text leftPlayerScoreText(font), rightPlayerScoreText(font);
	leftPlayerScoreText.setCharacterSize(50);
	leftPlayerScoreText.setPosition({ 500.f, 20.f });
	leftPlayerScoreText.setFillColor(sf::Color::White);

	rightPlayerScoreText.setCharacterSize(50);
	rightPlayerScoreText.setPosition({ 730.f, 20.f });
	rightPlayerScoreText.setFillColor(sf::Color::White);

	//--------------------------------------------------------------------------------------------------
	//PLAYER SCORES
	//--------------------------------------------------------------------------------------------------
	int leftPlayerScore = 100000, rightPlayerScore = 0;

	//--------------------------------------------------------------------------------------------------
	//PADDLE SETTINGS
	//--------------------------------------------------------------------------------------------------
	float paddleSpeed = 300.f;
	float paddleHeight = 100.f, paddleWidth = 20.f;
	sf::RectangleShape leftPaddle({ paddleWidth, paddleHeight });
	leftPaddle.setFillColor(sf::Color::White);
	leftPaddle.setPosition({ 50.f, 300.f });
	sf::RectangleShape rightPaddle({ paddleWidth, paddleHeight });
	rightPaddle.setFillColor(sf::Color::White);
	rightPaddle.setPosition({ WIDTH - paddleWidth - 50, 300.f });

	//--------------------------------------------------------------------------------------------------
	//BALL SETTINGS
	//--------------------------------------------------------------------------------------------------
	const float ballRadius = 10.f;
	const float ballDiameter = ballRadius * 2.f;
	sf::CircleShape ball(ballRadius);
	ball.setFillColor(sf::Color::Yellow);
	float ballSpeedX = 300.f, ballSpeedY = 200.f;
	ball.setPosition({ WIDTH / 2.f - ballRadius, HEIGHT / 2.f - ballRadius });

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

		paddleControl(rightPaddle, leftPaddle, paddleSpeed, dt, HEIGHT, paddleHeight);
		
		ballMovement(ball, ballSpeedX, ballSpeedY, dt, HEIGHT, ballDiameter);
		
		paddleCollision(leftPaddle, rightPaddle, ball, ballSpeedX);
		
		playerMiss(ball, leftPlayerScore, rightPlayerScore, ballRadius, ballDiameter, WIDTH, HEIGHT, ballSpeedY);

		updateScoreText(leftPlayerScoreText, rightPlayerScoreText, leftPlayerScore, rightPlayerScore);

		draw(window, leftPaddle, rightPaddle, ball, leftPlayerScoreText, rightPlayerScoreText);

	}
	return 0;
}

//--------------------------------------------------------------------------------------------------
//PADDLE CONTROL
//--------------------------------------------------------------------------------------------------
void paddleControl(sf::RectangleShape& rightPaddle, sf::RectangleShape& leftPaddle, float paddleSpeed,float dt, int HEIGHT, float paddleHeight)
{
	float rightPaddleY = rightPaddle.getPosition().y;
	float leftPaddleY = leftPaddle.getPosition().y;
	//RIGHT PADDLE CONTROL
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && rightPaddleY > 0) {
		rightPaddle.move({ 0, -paddleSpeed * dt });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && rightPaddleY < HEIGHT - paddleHeight) {
		rightPaddle.move({ 0, paddleSpeed * dt });
	}
	// LEFT PADDLE CONTROL
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && leftPaddleY > 0) {
		leftPaddle.move({ 0, -paddleSpeed * dt });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && leftPaddleY < HEIGHT - paddleHeight) {
		leftPaddle.move({ 0, paddleSpeed * dt });
	}
}

//--------------------------------------------------------------------------------------------------
//BALL MOVEMENT
//--------------------------------------------------------------------------------------------------
void ballMovement(sf::CircleShape& ball, float ballSpeedX, float& ballSpeedY, float dt, float HEIGHT, float ballDiameter)
{
	ball.move({ ballSpeedX * dt, ballSpeedY * dt });
	if (ball.getPosition().y <= 0 || ball.getPosition().y >= HEIGHT - ballDiameter) {
		ballSpeedY = -ballSpeedY;
	}
}

//--------------------------------------------------------------------------------------------------
//CHECK BALL - PADDLE COLLISION
//--------------------------------------------------------------------------------------------------
void paddleCollision(sf::RectangleShape& leftPaddle, sf::RectangleShape& rightPaddle, sf::CircleShape& ball, float& ballSpeedX)
{
	if (ballSpeedX < 0 && leftPaddle.getGlobalBounds().findIntersection(ball.getGlobalBounds()))
	{
		ballSpeedX = -ballSpeedX;
	}
	if (ballSpeedX > 0 && rightPaddle.getGlobalBounds().findIntersection(ball.getGlobalBounds()))
	{
		ballSpeedX = -ballSpeedX;
	}
}

//--------------------------------------------------------------------------------------------------
//IF PLAYER MISSES TO HIT THE BALL, UPDATE SCORE AND RESET BALL POSITION
//--------------------------------------------------------------------------------------------------
void playerMiss(sf::CircleShape& ball, int& leftPlayerScore, int& rightPlayerScore,float ballRadius, float ballDiameter, int WIDTH, int HEIGHT, float& ballSpeedY)
{
	if (ball.getPosition().x < 0 - ballDiameter)
	{
		rightPlayerScore++;
		ball.setPosition({ WIDTH / 2.f - ballRadius, HEIGHT / 2.f - ballRadius });
		ballSpeedY = 200.f;

	}
	if (ball.getPosition().x > WIDTH)
	{
		leftPlayerScore++;
		ball.setPosition({ WIDTH / 2.f - ballRadius, HEIGHT / 2.f - ballRadius });
		ballSpeedY = 200.f;
	}
}

//--------------------------------------------------------------------------------------------------
//UPDATE PLAYER SCORES TEXT
//--------------------------------------------------------------------------------------------------
void updateScoreText(sf::Text& leftPlayerScoreText, sf::Text& rightPlayerScoreText, int leftPlayerScore, int rightPlayerScore)
{
	leftPlayerScoreText.setString(std::to_string(leftPlayerScore));
	rightPlayerScoreText.setString(std::to_string(rightPlayerScore));
}

//--------------------------------------------------------------------------------------------------
// DRAW
//--------------------------------------------------------------------------------------------------
void draw(sf::RenderWindow& window, sf::RectangleShape& leftPaddle, sf::RectangleShape& rightPaddle, sf::CircleShape& ball, sf::Text& leftPlayerScoreText, sf::Text& rightPlayerScoreText)
{
	window.clear(sf::Color::Black);

	window.draw(leftPaddle);
	window.draw(rightPaddle);
	window.draw(ball);
	window.draw(leftPlayerScoreText);
	window.draw(rightPlayerScoreText);

	window.display();
}