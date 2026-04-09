/// <summary>
/// author Matej Marek February 2026
/// you need to change the above line or lose marks
/// Also don't have any member properties called Delete...
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )
// ignore this warning
// C:\SFML - 3.0.0\include\SFML\System\Exception.hpp(41, 47) : 
// warning C4275 : non dll - interface class 'std::runtime_error' used as base for dll - interface class 'sf::Exception'

/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the #endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const sf::Color ULTRAMARINE{ 5, 55,242,255 }; // const colour


const unsigned SPEED = 2.0f;


const unsigned WINDOW_HEIGHT = 720;
const unsigned WINDOW_WIDTH = 1280;

class Game
{
public:
	Game();
	~Game();
	void run();



private:

	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void checkKeyboardState();
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupTexts();
	void setupSprites();
	void setupAudio();
	void move();
	void resetShip(); 
	bool checkCollisionGround(sf::RectangleShape& ship, sf::RectangleShape& ground);
	bool checkCollisionPlatform(sf::RectangleShape& ship, sf::RectangleShape& platform);
	bool shipLanding(sf::RectangleShape& ship, sf::RectangleShape& platform);
	void fuelAnimation();
	void resetFuel();
	void keepShipOnScreen();
	void setupExplosion();
	void animateExplosion();


	sf::RectangleShape m_simpleShip; // simple ship
	sf::Vector2f m_simpleShipLocation; // vector for simple ship location
	sf::Angle m_simpleShipHeading{sf::degrees(60.0f)}; //heading for simple ship
	
	sf::Texture m_shipTexture; // texture for spaceship
	sf::Sprite m_shipSprite{m_shipTexture}; // sprite for spaceship

	sf::Texture m_landingPlatformTexture; // texture for landing platform
	sf::Sprite m_landingPlatformSprite{ m_landingPlatformTexture }; // sprite for landing platform
	sf::Vector2f m_landingPlatformLocation; // vector for landing platform location

	// three boolean to control movement of the ship
	bool m_engineOn{false};
	bool m_rotateLeft{false};
	bool m_rotateRight{false};

	const float ROTATION_SPEED = 0.4f;

	sf::RectangleShape m_simpleGround; // simple ground rectangle shape
	sf::Vector2f m_simpleGroundLocation; // vector for simple ground location

	sf::RectangleShape m_simplePlatform; // simple platform rectangle shape
	sf::Vector2f m_simplePlatformLocation; // vector for simple platform location

	sf::Texture m_earthTexture; // texture for earth background
	sf::Sprite m_earthSprite{ m_earthTexture }; // sprite for earth background
	sf::Vector2f m_earthBGLocation; // vector for earth background location

	sf::Vector2f m_gravity{ 0.0f, 0.02f }; //gravity force
	sf::Vector2f m_shipVelocity; // spaceship velocity

	sf::RectangleShape m_fuelBar; // fuel bar of the rocket
	sf::Vector2f m_fuelBarLocation; // vector for fuel bar location
	sf::RectangleShape m_fuelBarBackground; // background for fuel bar
	sf::Vector2f m_fuelBarBackgroundLocation; // vector for fuel bar background location
	bool m_hasFuel = true; // bollean if spaceship has fuel
	bool m_resetFuel = false; // boolean to reset fuel bar
	bool m_safeLanding = true; //boolean if the ship has landed safely or not

	sf::Texture m_explosionTexture; // texture for explosion
	sf::Sprite m_explosionSprite{m_explosionTexture}; // sprite for explosion
	sf::Vector2f m_explosionSpriteLocation; // vector for explosion sprite location
	sf::Vector2f m_explosionPosition; // vector for explosion position

	bool m_explodingGround{false}; // boolean if the ship is exploding or not
	bool m_explodingPlatform{false}; // boolean if the ship is exploding on the platform or not
	int m_explosionFrame{0}; // current frame
	float m_explosionFrameCounter{ 0.0f}; // frame counter for explosion
	float m_frameIncrement{0.6f}; // how fast the explosion animates

	sf::Vector2f m_xPositionGround; // vector for x position of the ship when it collides with the ground
	sf::Vector2f m_xPositionPlatform; // vector for x position of the ship when it collides with the platform

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_jerseyFont;// font used by message

	sf::SoundBuffer m_explosionSoundBuffer; // sound buffer for explosion sound
	sf::Sound m_explosionSound{m_explosionSoundBuffer}; // sound for explosion
	sf::SoundBuffer m_engineSoundBuffer; // sound buffer for engine sound
	sf::Sound m_engineSound {m_engineSoundBuffer}; // sound for engine

	bool m_isPlayingEngine = false; // boolean if engine sound is playing or not

	bool m_graphics = true;
	
	
	bool m_exitGame; // control exiting game

	

};

#pragma warning( pop ) 
#endif // !GAME_HPP

