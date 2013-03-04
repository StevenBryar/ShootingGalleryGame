#include "ShootingGalleryGame.h"
#include "common.h"
#include "constants.h"
#include "MessageHandler.h"

ShootingGalleryGame* ShootingGalleryGame::m_Instance = NULL;
ShootingGalleryGame* ShootingGalleryGame::instance(){
	if(!m_Instance){
		m_Instance = new ShootingGalleryGame();
	}
	return m_Instance;
}
void ShootingGalleryGame::cleanupInstance(){
	SafePtrRelease(m_Instance);
}

ShootingGalleryGame::ShootingGalleryGame(){}
ShootingGalleryGame::~ShootingGalleryGame(){}
void ShootingGalleryGame::loadContent(){}
void ShootingGalleryGame::initialize(){}

void ShootingGalleryGame::update(){

}
void ShootingGalleryGame::reset(){

}