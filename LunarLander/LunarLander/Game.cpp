/// <summary>
/// author Matej Marek February 2026
/// you need to change the above line or lose marks
/// </summary>


#include "Game.h"
#include <iostream>
#include <cmath>
#include <math.h>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the texts
/// load and setup the images
/// load and setup the sounds
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{WINDOW_WIDTH, WINDOW_HEIGHT}, 32U }, "SFML Game 3.0" },
	m_exitGame{false} //when true game will exit
{
	setupTexts(); // load font 
	setupSprites(); // load texture
	setupAudio(); // load sounds
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) // close window message 
		{
			m_exitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_exitGame = true; 
	}

	if (sf::Keyboard::Key::G == newKeypress->code)
	{
		m_graphics = !m_graphics;
	}
}

/// <summary>
/// Check if any keys are currently pressed
/// </summary>
void Game::checkKeyboardState()
{
	// booleans became true if the key is pressed and false if not

	m_engineOn    = m_hasFuel && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
		        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up));
		
	m_rotateLeft  = m_hasFuel && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
		        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left));

	m_rotateRight = m_hasFuel && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
		        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right));

	bool shouldPlayEngine = m_engineOn && m_hasFuel;

	if (shouldPlayEngine && !m_isPlayingEngine)
	{
		m_engineSound.play();
		m_isPlayingEngine = true;
	}
	else if (!shouldPlayEngine && m_isPlayingEngine)
	{
		m_engineSound.stop();
		m_isPlayingEngine = false;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		resetShip();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_exitGame = true; 
	}

	
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();
	if (m_exitGame)
	{
		m_window.close();
	}

	if(m_explodingGround)
	{
		float radians = m_simpleShipHeading.asRadians() - 1.5708f; // subtracting 90 degrees in radians to match explosion with the bottom of the ship
		m_explosionSprite.setPosition(sf::Vector2f{ m_xPositionGround.x ,WINDOW_HEIGHT - 100.0f});
		animateExplosion();
	}
	if(m_explodingPlatform && m_safeLanding == false)
	{
		float radians = m_simpleShipHeading.asRadians() - 1.5708f; // subtracting 90 degrees in radians to match explosion with the bottom of the ship
		m_explosionSprite.setPosition(sf::Vector2f{ m_xPositionPlatform.x ,WINDOW_HEIGHT - 135.0f});
		animateExplosion();
	}

	if(checkCollisionPlatform(m_simpleShip, m_simplePlatform))
	{
		if(shipLanding(m_simpleShip, m_simplePlatform))
		{
		}
		else
		{
			m_explosionPosition = m_simpleShipLocation;
			m_explosionSound.play();
			resetShip();
		}
		
	} else {
	move();

	}
	
	
	checkCollisionGround(m_simpleShip, m_simpleGround);
	
	fuelAnimation();
	resetFuel();
	keepShipOnScreen();
	// if(isKeyPressed(sf::Keyboard::Key::W))
	// {
	// 	m_engineSound.play();
	// }

}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{

	if (m_graphics)
	{
		m_window.draw(m_earthSprite);

		m_window.draw(m_simplePlatform);
		m_window.draw(m_fuelBarBackground);
		m_window.draw(m_fuelBar);
		m_window.draw(m_landingPlatformSprite);
		m_window.draw(m_shipSprite);
		
	}
	else
	{
		m_window.clear(ULTRAMARINE);
		m_window.draw(m_simpleGround);
		m_window.draw(m_simplePlatform);
		m_window.draw(m_fuelBarBackground);
		m_window.draw(m_fuelBar);
		m_window.draw(m_simpleShip);
	}

	if(m_explodingGround || m_explodingPlatform)
	{
		m_window.draw(m_explosionSprite);
	}
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupTexts()
{
	

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprites()
{
	setupExplosion();

	if(!m_shipTexture.loadFromFile("ASSETS/IMAGES/spaceship.png")) // if spaceship is not loading it will tell me
	{
		std::cout << "problem loading spaceship texture" << std::endl;
	}
	m_shipSprite.setTexture(m_shipTexture, true);
	m_shipSprite.setTextureRect(sf::IntRect{sf::Vector2i{0,0}, sf::Vector2i{24, 96}});
	m_shipSprite.setPosition(m_simpleShipLocation); // setting position of ship sprite to the same location as simple ship
	m_shipSprite.setOrigin(sf::Vector2f{ 12.0f, 16.0f });


	if (!m_earthTexture.loadFromFile("ASSETS/IMAGES/earth.png")) // if background is not loading it will tell me
	{
		std::cout << "problem loading earth texture" << std::endl;
	}
	m_earthSprite.setTexture(m_earthTexture, true);
	m_earthSprite.setTextureRect(sf::IntRect{ sf::Vector2i{0, 0}, sf::Vector2i{1280, 720} });
	m_earthBGLocation = sf::Vector2f{ 0.0f, 0.0f };
	m_shipSprite.setPosition(m_earthBGLocation); // setting position of earth background

	if (!m_landingPlatformTexture.loadFromFile("ASSETS/IMAGES/landingPlatform.png"))
	{
		std::cout << "problem loading landing platform texture" << std::endl;
	}
	m_landingPlatformSprite.setTexture(m_landingPlatformTexture, true);
	m_landingPlatformSprite.setTextureRect(sf::IntRect{ sf::Vector2i{0,0}, sf::Vector2i{150, 35} });
	m_landingPlatformLocation = sf::Vector2f{ (WINDOW_WIDTH / 2.0f) - 75, WINDOW_HEIGHT - 135.0f };
	m_landingPlatformSprite.setPosition(m_landingPlatformLocation);



	m_simpleShip.setFillColor(sf::Color::Green);
	m_simpleShip.setSize(sf::Vector2f{24.0f, 96.0f});
	m_simpleShipLocation = sf::Vector2f{300.0f, -50.0f};
	m_simpleShip.setPosition(m_simpleShipLocation);
	m_simpleShip.setRotation(m_simpleShipHeading);
	m_simpleShip.setOrigin(sf::Vector2f{ 12.0f, 16.0f});

	m_simpleGround.setFillColor(sf::Color::Red);
	m_simpleGround.setSize(sf::Vector2f{WINDOW_WIDTH, 100.0f });
	m_simpleGroundLocation = sf::Vector2f{0.0f, WINDOW_HEIGHT - 100.0f};
	m_simpleGround.setPosition(m_simpleGroundLocation);

	m_simplePlatform.setFillColor(sf::Color::Yellow);
	m_simplePlatform.setSize(sf::Vector2f{150.0f, 35.0f});
	m_simplePlatformLocation = sf::Vector2f{(WINDOW_WIDTH / 2.0f) - 75, WINDOW_HEIGHT - 135.0f};
	m_simplePlatform.setPosition(m_simplePlatformLocation);

	m_fuelBar.setFillColor(sf::Color::Green);
	m_fuelBar.setSize(sf::Vector2f{150.0f, 15.0f});
	m_fuelBarLocation = sf::Vector2f{ 110.0f, WINDOW_HEIGHT - 40.0f};
	m_fuelBar.setPosition(m_fuelBarLocation);
	m_fuelBar.setOrigin(sf::Vector2f{ 75.0f, 7.5f});

	m_fuelBarBackground.setFillColor(sf::Color::Black);
	m_fuelBarBackground.setSize(sf::Vector2f{160.0f, 25.0f});
	m_fuelBarBackgroundLocation = sf::Vector2f{ 110.0f, WINDOW_HEIGHT - 40.0f};
	m_fuelBarBackground.setPosition(m_fuelBarBackgroundLocation);
	m_fuelBarBackground.setOrigin(sf::Vector2f{ 80.0f, 12.5f});


}

/// <summary>
/// load sound file and assign buffers
/// </summary>
void Game::setupAudio()
{
	if(!m_explosionSoundBuffer.loadFromFile("ASSETS/AUDIO/explosion.wav")) // if explosion sound is not loading it will tell me
	{
		std::cout << "problem loading explosion sound" << std::endl;
	}
	m_explosionSound.setBuffer(m_explosionSoundBuffer);
	m_explosionSound.setVolume(25.0f); // setting volume to 25%

	if(!m_engineSoundBuffer.loadFromFile("ASSETS/AUDIO/rocketEngine.wav")) // if engine sound is not loading it will tell me
	{
		std::cout << "problem loading engine sound" << std::endl;
	}
	m_engineSound.setBuffer(m_engineSoundBuffer);
	m_engineSound.setLooping(true); // setting engine sound to loop
	//m_engineSound.play();
}

// move the ship based on user input and gravity
void Game::move()
{
	//dx and dy is a displacement or number of pixels the ship moves in a single frame
	//dt is delta time which is change in time
	float dt = 1.0f / 60.0f;
	

	// if statement to rotate anti-clockwise
	if (m_rotateLeft)
	{
		m_simpleShipHeading -= sf::degrees(ROTATION_SPEED);
	}

	// if statement to rotate clockwise
	if (m_rotateRight)
	{
		m_simpleShipHeading += sf::degrees(ROTATION_SPEED);
	}

	// if statement to be able to fly with rocket 'W' or 'Upp arrow' key in the direcion the ship is heading
	if (m_engineOn)
	{
		float radians = m_simpleShipHeading.asRadians();
		float dx = std::sin(radians) * SPEED * dt;
		float dy = -std::cos(radians) * SPEED * dt;
		m_shipVelocity += sf::Vector2f{ dx, dy };
		
	}
	
	m_shipVelocity += m_gravity;
	m_simpleShipLocation += m_shipVelocity;

	m_simpleShip.setPosition(m_simpleShipLocation);
	m_simpleShip.setRotation(m_simpleShipHeading);

	m_shipSprite.setPosition(m_simpleShipLocation);
	m_shipSprite.setRotation(m_simpleShipHeading);
}

// resets the spaceship to the starting position
void Game::resetShip()
{
	m_simpleShipLocation = sf::Vector2f{ 300.0f, -50.0f};
	m_simpleShipHeading = sf::degrees(60.0f);
	m_simpleShip.setPosition(m_simpleShipLocation);
	m_simpleShip.setRotation(m_simpleShipHeading);
	m_shipVelocity = sf::Vector2f{ 0.0f, 0.0f };
	m_resetFuel = true;

}

// checks for collisions between the ship and the ground
bool Game::checkCollisionGround(sf::RectangleShape& t_ship, sf::RectangleShape& t_ground)
{
	bool collisionGround = false; // boolean if there is a collision or not
	sf::FloatRect shipBounds = t_ship.getGlobalBounds();
	sf::FloatRect groundBounds = t_ground.getGlobalBounds();

	std::optional<sf::FloatRect> overlap= shipBounds.findIntersection(groundBounds);
	if(overlap.has_value()) // if there is an intersection between the ship and the ground then collisionResult becomes true
	{
		collisionGround = true;
	}
	if(collisionGround)
	{
		m_explodingGround = true;
		m_xPositionGround.x = m_simpleShipLocation.x;
		m_explosionSound.play();
		resetShip();

		
	}
	// if(collisionGround) // if collisionResult is true then reset the ship to the starting position
	// {
	// 	resetShip();
	// }
	

	return collisionGround;
}

// checks for collisions between the ship and the platform
bool Game::checkCollisionPlatform(sf::RectangleShape& t_ship, sf::RectangleShape& t_platform)
{
	bool collisionPlatform = false; // booleand if there is a collision or not
	sf::FloatRect shipBounds = t_ship.getGlobalBounds();
	sf::FloatRect platformBounds = t_platform.getGlobalBounds();
	

	std::optional<sf::FloatRect> overlapPlatform = shipBounds.findIntersection(platformBounds);
	if(overlapPlatform.has_value())
	{
		collisionPlatform = true;
		
		if(collisionPlatform)
		{
			m_explodingPlatform = true;
			m_xPositionPlatform.x = m_simpleShipLocation.x;
			
		
		}
	}
	


	return collisionPlatform;
}

bool Game::shipLanding(sf::RectangleShape &ship, sf::RectangleShape &platform)
{
    

	// if the ship's velocity is greater than 1.0f or the ship's heading is greater than 10 degrees
	//or less than -10 degrees then the landing bollean is false
	if(m_shipVelocity.y > 0.8f || m_simpleShipHeading.asDegrees() > 10.0f || m_simpleShipHeading.asDegrees() < -10.0f) 
	{
		m_safeLanding = false;
	}
	else
	{
		m_safeLanding = true;
	}
	
	return m_safeLanding;
}

void Game::fuelAnimation()
{
	// if the W or Up is pressed then the fuel bar will decrease by 0.3f every frame
	if(isKeyPressed(sf::Keyboard::Key::W) || isKeyPressed(sf::Keyboard::Key::Up))
	{
		m_fuelBar.setSize(sf::Vector2f{m_fuelBar.getSize().x - 0.27f, 15.0f});
	}

	// if the fuel bar is zero then the spaceship has no fuel and user cannot control the ship anymore
	if(m_fuelBar.getSize().x <= 0.0f)
	{
		m_fuelBar.setSize(sf::Vector2f{0.0f, 15.0f});
		m_hasFuel = false;
	}
	if (m_fuelBar.getSize().x < (100.0f))
	{
		m_fuelBar.setFillColor(sf::Color::Yellow);
	}
	if (m_fuelBar.getSize().x < 50.0f)
	{
		m_fuelBar.setFillColor(sf::Color::Red);
	}
}

void Game::resetFuel()
{
	if (m_resetFuel)
	{
		m_fuelBar.setSize(sf::Vector2f{ 150.0f , 15.0f });
		m_fuelBar.setFillColor(sf::Color::Green);
		m_hasFuel = true;
		m_resetFuel = false;
	}
}


void Game::keepShipOnScreen()
{
	if (m_simpleShipLocation.x < 0.0f)
	{
		m_simpleShipLocation.x = WINDOW_WIDTH;
	}
	else if (m_simpleShipLocation.x > WINDOW_WIDTH)
	{
		m_simpleShipLocation.x = 0.0f;
	}
}

void Game::setupExplosion()
{
	if(!m_explosionTexture.loadFromFile("ASSETS/IMAGES/explosion.png")) // if explosion texture is not loading it will tell me
	{
		std::cout << "problem loading explosion" << std::endl;
	}
	m_explosionSprite.setTextureRect(sf::IntRect{sf::Vector2i{0,0}, sf::Vector2i{100, 100,}});
	m_explosionSprite.setOrigin(sf::Vector2f{50.0f, 50.0f});
	m_explosionSprite.setPosition(m_xPositionGround); // setting position of explosion to the same location as simple ships bottom
}

void Game::animateExplosion()
{
	int frame;
	const int ROWS = 6;
	const int COLS = 8;
	const int SIZE = 100;
	int row;
	int col;

	m_explosionFrameCounter += m_frameIncrement;
	frame = static_cast<int>(m_explosionFrameCounter);
	if(frame > 47)
	{
		m_explodingGround = false;
		m_explodingPlatform = false;
		frame = 0;
		m_explosionFrameCounter = 0.0f;
	}
	if(frame != m_explosionFrame)
	{
		col = frame % COLS;
		row = frame / COLS;
		m_explosionSprite.setTextureRect(sf::IntRect{sf::Vector2i{col * SIZE, row * SIZE}, sf::Vector2i{SIZE, SIZE}});
	}
}
