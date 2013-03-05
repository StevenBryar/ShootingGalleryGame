#include "ShootingGalleryGame.h"
#include "common.h"
#include "constants.h"
#include "MessageHandler.h"
#include "inputManager.h"
#include "TextManager.h"
#include "gameObject.h"
#include "uiButton.h"
#include "Target.h"
#include "ShootingGameConstants.h"
#include "2dSprite.h"
#include "spriteManager.h"
#include "uiButton.h"

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

ShootingGalleryGame::ShootingGalleryGame(){
	loadContent();
	reset();
}
ShootingGalleryGame::~ShootingGalleryGame(){
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		SafePtrRelease(m_GameObjects);
	}
	if(m_HighScores){
		SafeVectorDelete<HighScore>(*m_HighScores);
		SafePtrRelease(m_HighScores);
	}
}
void ShootingGalleryGame::loadContent(){
	SpriteManager::instance()->loadTexture("MainMenuBack.png");
	SpriteManager::instance()->loadTexture("PlayButtonD.png");
	SpriteManager::instance()->loadTexture("PlayButtonH.png");
	SpriteManager::instance()->loadTexture("PlayButtonS.png");
}
void ShootingGalleryGame::initialize(){
	m_GameObjects = new std::vector<GameObject*>();
	m_HighScores = new std::vector<HighScore*>();
	m_Score = 0;
	m_CurrentState = None;
	m_Background = NULL;
}

void ShootingGalleryGame::update(){
	MessageHandler::Instance()->update();
	(this->*SGUpdate)();
}
void ShootingGalleryGame::reset(){
	initialize();
	changeState(MainMenu);
}

void ShootingGalleryGame::changeState(const SGGameStates& state){
	if(state != m_CurrentState){
		switch(m_CurrentState){
		case MainMenu:
			endMainMenu();
			break;
		case GamePlay:
			endGame();
			break;
		case HighScoreScreen:
			endHighScore();
			break;
		case NewHighScore:
			endNewHighScore();
			break;
		case PlayAgain:
			endPlayAgain();
			break;
		}
		switch(state){
		case MainMenu:
			beginMainMenu();
			SGUpdate = &ShootingGalleryGame::mainMenu;
			break;
		case GamePlay:
			beginGame();
			SGUpdate = &ShootingGalleryGame::gamePlay;
			break;
		case HighScoreScreen:
			beginHighScore();
			SGUpdate = &ShootingGalleryGame::highScore;
			break;
		case NewHighScore:
			beginNewHighScore();
			SGUpdate = &ShootingGalleryGame::newHighScore;
			break;
		case PlayAgain:
			beginPlayAgain();
			SGUpdate = &ShootingGalleryGame::playAgain;
			break;
		}
		m_CurrentState = state;
	}
}

void ShootingGalleryGame::gamePlay(){
	//update objects.
	//if m_SetToSpawn == false
		//send message to game to spawn targets at random time between 1 - 4 seconds.
	//decrease time left.
	//if time left >= 0 check for high score.
		//if highscore change to newHighScore mode.
		//else switch to play again screen.
}
void ShootingGalleryGame::mainMenu(){
	//update m_GameObjects.
	for(int i = 0;i < m_GameObjects->size();i++){
		(*m_GameObjects)[i]->update();
	}
}
void ShootingGalleryGame::highScore(){
	//update objects.
}

void ShootingGalleryGame::newHighScore(){
	//update m_GameObjects.
}
void ShootingGalleryGame::playAgain(){
	//update m_GameObjects.
}

Target* ShootingGalleryGame::spawnTarget(){
	Target* target = NULL;
	//randomly choose between several different target setups.
	//randomly choose what the point type is(+-*/)chances(+:6,-:3,*:2,/:1)
	//randomly choose location on the play area.
	//randomly chose color of sprite.(+*:B-8,R-2),(-/:B-2,R-8)
	return target;
}

void ShootingGalleryGame::beginMainMenu(){
	//create play button.
	//create highscore button.
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	UiButton* playB = new UiButton(240,350,128,128,"PlayButtonD.png","PlayButtonS.png","PlayButtonH.png",
									FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,NULL,&changeGameState,(void*)GamePlay);
	m_GameObjects->push_back(playB);
}
void ShootingGalleryGame::endMainMenu(){
	SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		SafePtrRelease(m_GameObjects);
	}
}

void ShootingGalleryGame::beginGame(){
	//create random stationary targets to start.
	//create quit button.
	//create background image.
	//set score to 0.
}
void ShootingGalleryGame::endGame(){
	SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		SafePtrRelease(m_GameObjects);
	}
}

void ShootingGalleryGame::beginHighScore(){
	//load highscore.
	//create background image.
	//create back button.
	//create text on screen matching high scores.
}
void ShootingGalleryGame::endHighScore(){
	SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		SafePtrRelease(m_GameObjects);
	}
}
void ShootingGalleryGame::beginNewHighScore(){
	//create three UiListMenu objects with there entries set to the letters of the alphabet.
	//create a Text object to represent the high score.
	//create finished button.
	//create background image.
	//Create new High Score Text as a title.
}
void ShootingGalleryGame::endNewHighScore(){
	//delete all text.
		SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		SafePtrRelease(m_GameObjects);
	}
}
void ShootingGalleryGame::beginPlayAgain(){
	//Create Play Again? Text.
	//Create play again Button.
	//Create quit to main Button.
	//Create background image.
}
void ShootingGalleryGame::endPlayAgain(){
	//delete all text.
	SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		SafePtrRelease(m_GameObjects);
	}
}

void ShootingGalleryGame::saveHighScores(){
	//take the m_HighScores vector and save them to the "highScores.hs" file.
}
void ShootingGalleryGame::loadHighScores(){
	//load the "highScores.hs" file into m_HighScores.
}

void ShootingGalleryGame::mouseInputCalback(const inputEvent& event,const int& x,const int& y){

}

void ShootingGalleryGame::handleMessage(const Message& msg){
	switch(msg.type){
	case CHANGE_STATE:{
			int s = (int)msg.extraInfo;
			changeState((SGGameStates)s);
		}
		break;
	case SPAWN_TARGETS:{
			if(m_CurrentState == GamePlay){
				int n = (int)msg.extraInfo;
				for(int i = 0;i < n;i++){
					m_GameObjects->push_back(spawnTarget());
				}
			}
		}
		break;
	default:
		break;
	}
}

void pause(void* param){}
void changeGameState(void* state){
	MessageHandler::Instance()->createMessage(CHANGE_STATE,ShootingGalleryGame::instance(),
												ShootingGalleryGame::instance(),state,0);
}