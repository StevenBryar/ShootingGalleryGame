#include "ShootingGalleryGame.h"
#include "common.h"
#include "constants.h"
#include "MessageHandler.h"
#include "inputManager.h"
#include "TextManager.h"
#include "gameObject.h"
#include "uiButton.h"
#include "uiListMenu.h"
#include "Target.h"
#include "ShootingGameConstants.h"
#include "2dSprite.h"
#include "spriteManager.h"
#include "uiButton.h"
#include "Text.h"
#include "Vector3.h"
#include <fstream>

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
	MessageHandler::Instance()->cleanUpInstance();
	TextManager::instance()->cleanupInstance();
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		SafePtrRelease(m_GameObjects);
	}
	if(m_HighScores){
		SafePtrRelease(m_HighScores);
	}
}
void ShootingGalleryGame::loadContent(){
	SpriteManager::instance()->loadTexture("MainMenuBack.png");
	SpriteManager::instance()->loadTexture("PlayButtonD.png");
	SpriteManager::instance()->loadTexture("PlayButtonH.png");
	SpriteManager::instance()->loadTexture("PlayButtonS.png");
	TextManager::instance()->loadFont("tfa_squaresans.ttf");
}
void ShootingGalleryGame::initialize(){
	m_GameObjects = new std::vector<GameObject*>();
	m_HighScores = new std::vector<HighScore>();
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

bool ShootingGalleryGame::getPaused(){return m_Paused;}
void ShootingGalleryGame::setPaused(const bool& paused){m_Paused = paused;}

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
	if(!m_Paused){
		for(int i = 0;i < m_GameObjects->size();i++){
			(*m_GameObjects)[i]->update();
		}
	//if m_SetToSpawn == false
		//send message to game to spawn targets at random time between 1 - 4 seconds.
	//decrease time left.
	//if time left >= 0 check for high score.
		//if highscore change to newHighScore mode.
		//else switch to play again screen.
	}
}
void ShootingGalleryGame::mainMenu(){
	for(int i = 0;i < m_GameObjects->size();i++){
		(*m_GameObjects)[i]->update();
	}
}
void ShootingGalleryGame::highScore(){
	for(int i = 0;i < m_GameObjects->size();i++){
		(*m_GameObjects)[i]->update();
	}
}

void ShootingGalleryGame::newHighScore(){
	for(int i = 0;i < m_GameObjects->size();i++){
		(*m_GameObjects)[i]->update();
	}
}
void ShootingGalleryGame::playAgain(){
	for(int i = 0;i < m_GameObjects->size();i++){
		(*m_GameObjects)[i]->update();
	}
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
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	UiButton* playB = new UiButton(240,350,128,128,"PlayButtonD.png","PlayButtonS.png","PlayButtonH.png",
									FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,NULL,&changeGameState,(void*)GamePlay);
	m_GameObjects->push_back(playB);

	UiButton* highScoreButton = new UiButton(700,350,128,128,"PlayButtonD.png","PlayButtonS.png","PlayButtonH.png",
									FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,NULL,&changeGameState,(void*)HighScoreScreen);
	m_GameObjects->push_back(highScoreButton);
}
void ShootingGalleryGame::endMainMenu(){
	SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		m_GameObjects->clear();
	}
}

void ShootingGalleryGame::beginGame(){
	m_Score = 0;
	for(int i = 0;i < 10;i++){
		//m_GameObjects->push_back(spawnTarget());
	}
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	UiButton* quitB = new UiButton(240,350,128,128,"PlayButtonD.png","PlayButtonS.png",
									FIRE_ON_RELEASED,NULL,&changeGameState,(void*)GamePlay);
	m_GameObjects->push_back(quitB);

	UiButton* pauseB = new UiButton(700,350,128,128,"PlayButtonD.png","PlayButtonS.png",
									FIRE_ON_RELEASED,NULL,&pause,NULL);
	m_GameObjects->push_back(pauseB);
}
void ShootingGalleryGame::endGame(){
	SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		m_GameObjects->clear();
	}
}

void ShootingGalleryGame::beginHighScore(){
	loadHighScores();
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	UiButton* backButton = new UiButton(100,100,128,128,"PlayButtonD.png","PlayButtonS.png","PlayButtonH.png",
									FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,NULL,&changeGameState,(void*)MainMenu);
	m_GameObjects->push_back(backButton);
	Vector3 c(170,80,100);
	loadHighScores();
	for(int i = 0;i < m_HighScores->size();i++){
		TextManager::instance()->createText(
			(*m_HighScores)[i].m_Name,"tfa_squaresans.ttf",30,c,255,40,10+(i*50),0,false,0);
		TextManager::instance()->createText(
			intToString((*m_HighScores)[i].m_HighScore),"tfa_squaresans.ttf",30,c,255,500,10+(i*50),0,false,0);
	}
}
void ShootingGalleryGame::endHighScore(){
	TextManager::instance()->deleteAllText();
	SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		m_GameObjects->clear();
	}
}
void ShootingGalleryGame::beginNewHighScore(){
	HighScore* score = new HighScore();
	score->m_HighScore = m_Score;
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	UiButton* finishedB = new UiButton(400,400,128,128,"PlayButtonD.png","PlayButtonS.png","PlayButtonH.png",
									FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,NULL,&saveHighScore,(void*)score);
	m_GameObjects->push_back(finishedB);
	Vector3 c(170,80,100);
	TextManager::instance()->createText("YourScore: ","tfa_squaresans.ttf",30,c,255,255,200,false,0,0);
	TextManager::instance()->createText(intToString(m_Score),"tfa_squaresans.ttf",30,c,255,500,200,false,0,0);
	//create three UiListMenu objects with there entries set to the letters of the alphabet
	UiButton* scrollUp = new UiButton(100,100,128,128,"PlayButtonD.png","PlayButtonS.png",
									FIRE_ON_RELEASED,NULL,&scrollUpLetterSelect,NULL);
	UiButton* scrollDown = new UiButton(500,100,128,128,"PlayButtonD.png","PlayButtonS.png",
									FIRE_ON_RELEASED,NULL,&scrollDownLetterSelect,NULL);

	UiListMenu* letter1 = new UiListMenu(200,150,NULL,scrollUp,scrollDown,50,50,1,50);
	letter1->setWidth(150);
	letter1->setHeight(200);
	Text* text = NULL;
	for(int i = 0;i < LETTERS_NUMBERS_COUNT;i++){
		Text* text = TextManager::instance()->createText(LETTERS_NUMBERS[i],"tfa_squaresans.ttf",120,c,255,0,0,0,false,0);
		letter1->addEntry(text);
	}
	m_GameObjects->push_back(letter1);
}
void ShootingGalleryGame::endNewHighScore(){
	TextManager::instance()->deleteAllText();
		SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		m_GameObjects->clear();
	}
}
void ShootingGalleryGame::beginPlayAgain(){
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	UiButton* playAgainB = new UiButton(300,300,128,128,"PlayButtonD.png","PlayButtonS.png","PlayButtonH.png",
									FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,NULL,&changeGameState,(void*)GamePlay);
	m_GameObjects->push_back(playAgainB);
	UiButton* quitB = new UiButton(600,300,128,128,"PlayButtonD.png","PlayButtonS.png","PlayButtonH.png",
									FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,NULL,&changeGameState,(void*)MainMenu);
	m_GameObjects->push_back(quitB);

	Vector3 c(170,80,100);
	TextManager::instance()->createText("YourScore: ","tfa_squaresans.ttf",30,c,255,255,200,false,0,0);
	TextManager::instance()->createText(intToString(m_Score),"tfa_squaresans.ttf",30,c,255,500,200,false,0,0);
}
void ShootingGalleryGame::endPlayAgain(){
	SpriteManager::instance()->deleteSprite(m_Background);
	TextManager::instance()->deleteAllText();
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		m_GameObjects->clear();
	}
}

void ShootingGalleryGame::saveHighScores(){
	std::string path = "HighScores.hs";
	std::ifstream file(path);
	if(file){
		file.close();
		if(remove(path.c_str()) != 0){
			std::cout << "Can't save scores";
			return;
		}
	}
	std::fstream outFile("HighScores.hs",std::ios::out,std::ios::binary);
	for(int i = 0;i < m_HighScores->size();i++){
		outFile.write((*m_HighScores)[i].m_Name.c_str(),HIGHSCORE_NAME_LENGTH);
		outFile.write((char*)&((*m_HighScores)[i].m_HighScore),sizeof(int));
	}
	outFile.close();
	//take the m_HighScores vector and save them to the "highScores.hs" file.
}
void ShootingGalleryGame::loadHighScores(){
	std::fstream file("HighScores.hs",std::ios::in | std::ios::binary);
	if(!file){
		if(m_HighScores->size() > 0){
			saveHighScores();
			return;
		}
		else{
			for(int i = 0;i < MAX_HIGH_SCORES;i++){
				HighScore s;
				s.m_HighScore = 100;
				s.m_Name = "AAA";
				m_HighScores->push_back(s);
			}
			saveHighScores();
			return;
		}
	}
	HighScore s;
	char* buffer;
	for(int i = 0;i < MAX_HIGH_SCORES;i++){
		buffer = new char[HIGHSCORE_NAME_LENGTH];
		file.read(buffer,HIGHSCORE_NAME_LENGTH);
		s.m_Name = std::string(buffer,HIGHSCORE_NAME_LENGTH);
		file.read((char*)&(s.m_HighScore),sizeof(int));
		delete[] buffer;
		m_HighScores->push_back(s);
	}
	file.close();
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

void pause(void* param){ShootingGalleryGame::instance()->setPaused(!ShootingGalleryGame::instance()->getPaused());}

void changeGameState(void* state){
	MessageHandler::Instance()->createMessage(CHANGE_STATE,ShootingGalleryGame::instance(),
												ShootingGalleryGame::instance(),state,0);
}
void saveHighScore(void* highScore){}

void scrollUpLetterSelect(void* listMenu){
	UiListMenu* menu = (UiListMenu*)listMenu;
	menu->scrollUp();
	menu->setSelectedEntry(menu->getSelecedEntryNumber()-1);
}
void scrollDownLetterSelect(void* listMenu){
	UiListMenu* menu = (UiListMenu*)listMenu;
	menu->scrollDown();
	menu->setSelectedEntry(menu->getSelecedEntryNumber()+1);
}