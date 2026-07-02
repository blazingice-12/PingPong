#include <SFML/Graphics.hpp>

int main()
{
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

	const int WIDTH = 1280, HEIGHT = 720;

	//--------------------------------------------------------------------------------------------------
	//PLAYER SCORES
	//--------------------------------------------------------------------------------------------------
	int leftPlayerScore = 0, rightPlayerScore = 0;

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
		float rightPaddleY = rightPaddle.getPosition().y;
		float leftPaddleY = leftPaddle.getPosition().y;
		window.clear(sf::Color::Black);


		//--------------------------------------------------------------------------------------------------
		//PADDLE CONTROL
		//--------------------------------------------------------------------------------------------------
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

		//--------------------------------------------------------------------------------------------------
		//BALL MOVEMENT
		//--------------------------------------------------------------------------------------------------
		ball.move({ ballSpeedX * dt, ballSpeedY * dt });
		if (ball.getPosition().y <= 0 || ball.getPosition().y >= HEIGHT - ballDiameter) {
			ballSpeedY = -ballSpeedY;
		}

		//--------------------------------------------------------------------------------------------------
		//CHECK BALL AND PADDLE COLLISION OR IF BALL MISSES ANY ONE PADDLE
		//--------------------------------------------------------------------------------------------------
		//PADDLE COLLISION
		if (ballSpeedX < 0 && leftPaddle.getGlobalBounds().findIntersection(ball.getGlobalBounds()))
		{
			ballSpeedX = -ballSpeedX;
		}
		if (ballSpeedX > 0 && rightPaddle.getGlobalBounds().findIntersection(ball.getGlobalBounds()))
		{
			ballSpeedX = -ballSpeedX;
		}

		//IF BALL MISSES A PADDLE
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

		//--------------------------------------------------------------------------------------------------
		//UPDATE PLAYER SCORES TEXT
		//--------------------------------------------------------------------------------------------------
		leftPlayerScoreText.setString(std::to_string(leftPlayerScore));
		rightPlayerScoreText.setString(std::to_string(rightPlayerScore));

		//--------------------------------------------------------------------------------------------------
		// DRAWING
		//--------------------------------------------------------------------------------------------------
		window.draw(leftPaddle);
		window.draw(rightPaddle);
		window.draw(ball);
		window.draw(leftPlayerScoreText);
		window.draw(rightPlayerScoreText);

		window.display();
	}
	return 0;
}