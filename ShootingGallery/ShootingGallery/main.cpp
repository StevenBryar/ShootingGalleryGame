#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "InputManager.h"
#include "constants.h"
#include "InputListener.h"
#include "Renderer.h"
#include "Camera.h"
#include "SpriteManager.h"
#include "GameObject.h"
#include "2dSprite.h"
#include "Util.h"
#include "common.h"
#include "ShootingGalleryGame.h"

void updateMouse(sf::Window* window);

int main(){
	sf::RenderWindow* window = new sf::RenderWindow
		(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT),"ShootinGallery",sf::Style::Titlebar);
	window->setFramerateLimit(FRAMES_PER_SECOND);
	sf::Clock theClock;
	double lastUpdateTime = theClock.getElapsedTime().asSeconds();
	bool focus = true;
	sf::View* view = new sf::View(sf::FloatRect(0,0,WINDOW_WIDTH,WINDOW_HEIGHT));
	//Game loop
	while(window->isOpen()){
		if(focus){
			updateMouse(window);
		}
		Util::instance()->update();
		sf::Event event;
		InputManager::instance()->update();
		while(window->pollEvent(event)){
			switch(event.type){
			case sf::Event::Closed:
				SpriteManager::instance()->cleanupInstance();
				InputManager::instance()->cleanUpInstance();
				ShootingGalleryGame::instance()->cleanupInstance();
				Renderer::instance()->cleanupInstance();
				Util::instance()->cleanUpInstance();
				SafePtrRelease(view);
				window->close();
				SafePtrRelease(window);
				return 0;
			case sf::Event::LostFocus:
			case sf::Event::GainedFocus:
				focus = !focus;
				break;
			case sf::Event::MouseMoved:
				if(focus){
					InputManager::instance()->handleMouseInput
						(MOUSE_MOVED,sf::Mouse::getPosition(*window).x,
						sf::Mouse::getPosition(*window).y);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if(focus){
					if(event.mouseButton.button == sf::Mouse::Left){
						InputManager::instance()->handleMouseInput
						(MOUSE_LB_RELEASED,sf::Mouse::getPosition(*window).x,
						sf::Mouse::getPosition(*window).y);
					}
					else if(event.mouseButton.button == sf::Mouse::Right){
						InputManager::instance()->handleMouseInput
						(MOUSE_RB_RELEASED,sf::Mouse::getPosition(*window).x,
						sf::Mouse::getPosition(*window).y);
					}
				}
				break;
			case sf::Event::MouseButtonPressed:
				if(focus){
					if(event.mouseButton.button == sf::Mouse::Left){
						InputManager::instance()->handleMouseInput
							(MOUSE_LB_PRESSED,sf::Mouse::getPosition(*window).x,
							sf::Mouse::getPosition(*window).y);
					}
				}
				break;
			case sf::Event::KeyReleased:
				if(focus){
					if(event.key.code != sf::Keyboard::Escape){
						InputManager::instance()->handleKeyInput(KEY_RELEASED,(keyType)event.key.code);
					}
					else{
						SpriteManager::instance()->cleanupInstance();
						InputManager::instance()->cleanUpInstance();
						ShootingGalleryGame::instance()->cleanupInstance();
						Renderer::instance()->cleanupInstance();
						Util::instance()->cleanUpInstance();
						SafePtrRelease(view);
						window->close();
						SafePtrRelease(window);
						return 0;
					}
				}
			}
		}
		ShootingGalleryGame::instance()->update();
		SpriteManager::instance()->update();
		Renderer::instance()->render(window,view);
	}
	return 0;
}

void updateMouse(sf::Window* window){
		if(sf::Mouse::getPosition(*window).x < 0){
			sf::Mouse::setPosition(sf::Vector2i(0,sf::Mouse::getPosition(*window).y),(*window));
		}
		else if(sf::Mouse::getPosition(*window).x > WINDOW_WIDTH){
			sf::Mouse::setPosition(sf::Vector2i(WINDOW_WIDTH-2,sf::Mouse::getPosition(*window).y),(*window));
		}
		if(sf::Mouse::getPosition(*window).y < 0){
			sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition(*window).x,0),(*window));
		}
		else if(sf::Mouse::getPosition(*window).y > WINDOW_HEIGHT){
			sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition(*window).x,WINDOW_HEIGHT-2),(*window));
		}
}
