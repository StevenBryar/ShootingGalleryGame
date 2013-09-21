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
#include <stdlib.h>
#include <time.h>
#include "Util.h"

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
	changeState(None);
	MessageHandler::Instance()->cleanUpInstance();
	TextManager::instance()->cleanupInstance();
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		SafePtrRelease(m_GameObjects);
	}
	if(m_HighScores){
		SafePtrRelease(m_HighScores);
	}
	SafePtrRelease(m_Letter1);
	SafePtrRelease(m_Letter2);
	SafePtrRelease(m_Letter3);
}
void ShootingGalleryGame::loadContent(){
	SpriteManager::instance()->loadTexture("MainMenuBack.png");
	SpriteManager::instance()->loadTexture("PlayButtonD.png");
	SpriteManager::instance()->loadTexture("PlayButtonH.png");
	SpriteManager::instance()->loadTexture("PlayButtonS.png");
	SpriteManager::instance()->loadTexture("UpArrow.png");
	SpriteManager::instance()->loadTexture("UpArrowS.png");
	SpriteManager::instance()->loadTexture("DownArrow.png");
	SpriteManager::instance()->loadTexture("DownArrowS.png");
	SpriteManager::instance()->loadTexture("Finished.png");
	SpriteManager::instance()->loadTexture("FinishedS.png");
	SpriteManager::instance()->loadTexture("HighScoreD.png");
	SpriteManager::instance()->loadTexture("HighScoreH.png");
	SpriteManager::instance()->loadTexture("HighScoreS.png");
	SpriteManager::instance()->loadTexture("quitS.png");
	SpriteManager::instance()->loadTexture("quitD.png");
	SpriteManager::instance()->loadTexture("target.png");
	TextManager::instance()->loadFont("tfa_squaresans.ttf");
}
void ShootingGalleryGame::initialize(){
	m_GameObjects = new std::vector<GameObject*>();
	m_HighScores = new std::vector<HighScore>();
	m_Score = 0;
	m_CurrentState = None;
	m_Background = NULL;
	m_Letter1 = NULL;
	m_Letter2 = NULL;
	m_Letter3 = NULL;
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_RELEASED);
	srand(time(0));
}

void ShootingGalleryGame::update(){
	MessageHandler::Instance()->update();
	(this->*SGUpdate)();
}
void ShootingGalleryGame::reset(){
	initialize();
	changeState(MainMenu);
}
UiListMenu* ShootingGalleryGame::getLetter1() const{return m_Letter1;}
UiListMenu* ShootingGalleryGame::getLetter2() const{return m_Letter2;}
UiListMenu* ShootingGalleryGame::getLetter3() const{return m_Letter3;}
std::vector<HighScore>* ShootingGalleryGame::getHighScores() const{return m_HighScores;}
void ShootingGalleryGame::setHighScores(std::vector<HighScore>* scores){m_HighScores = scores;}

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
	std::vector<Target*> targetsToDelete;
	for(int i = 0;i < m_GameObjects->size();i++){
		(*m_GameObjects)[i]->update();
		if((*m_GameObjects)[i]->getType() == "Target"){
			if(!Util::instance()->rectIntersection(
				(*m_GameObjects)[i]->getPositionX(),
				(*m_GameObjects)[i]->getPositionY(),
				((*m_GameObjects)[i]->getPositionX()+(*m_GameObjects)[i]->getWidth()),
				((*m_GameObjects)[i]->getPositionY()+(*m_GameObjects)[i]->getHeight()),
				0,
				0,
				WINDOW_WIDTH,
				WINDOW_HEIGHT)){
					targetsToDelete.push_back((Target*)(*m_GameObjects)[i]);
			}
		}
	}
	if(m_SetToSpawn == false){
		int spawnTime = (rand() % 200)+200;
		MessageHandler::Instance()->createMessage(SPAWN_TARGETS,this,this,(void*)6,spawnTime);
		m_SetToSpawn = true;
	}
	m_TimeRemaining -= Util::instance()->getDelta();
	if(m_TimeRemaining <= 0){
		loadHighScores();
		for(int i = 0;i < m_HighScores->size();i++){
			if((*m_HighScores)[i].m_HighScore <= m_Score){
				changeState(NewHighScore);
				return;
			}
		}
		changeState(PlayAgain);
		return;
	}
	for(int i = 0;i < targetsToDelete.size();i++){
		for(int j = 0;j < m_GameObjects->size();j++){
			if(targetsToDelete[i] == (*m_GameObjects)[j]){
				SafePtrRelease(targetsToDelete[i]);
				m_GameObjects->erase(m_GameObjects->begin()+j);
				break;
			}
		}
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
	m_Letter1->update();
	m_Letter2->update();
	m_Letter3->update();
}
void ShootingGalleryGame::playAgain(){
	for(int i = 0;i < m_GameObjects->size();i++){
		(*m_GameObjects)[i]->update();
	}
}

Target* ShootingGalleryGame::spawnTarget(){
	Target* target = NULL;
	m_SetToSpawn = false;
	unsigned int targetType = 0;
	PointType pT = Plus;
	int setups[] = {2,4,8,16};
	PointType types[] = {Plus,Plus,Plus,Plus,Plus,Plus,Minus,Minus,Minus,Multi,Multi,Divi};
	int values[] = {20,20,60,20,120,60,20,60,400,120,20,20,60,120,200,400,800};
	int addSubValues = 17;
	int divMultiMax = 4;
	int numOfSetups = 4;
	int numOfTypeChances = 12;
	int randSetup;
	int value = 0;
	for(int i = 0;i < 4;i++){
		randSetup = (rand() % numOfSetups);
		if(setups[randSetup] == 0){continue;}
		targetType |= setups[randSetup];
		if(setups[randSetup] == 2 || setups[randSetup] == 8){
			setups[randSetup+1] = 0;
		}
		else if(setups[randSetup] == 4 || setups[randSetup] == 16){
			setups[randSetup-1] = 0;
		}
		setups[randSetup] = 0;
	}
	randSetup = (rand() % 50);
	if(randSetup < 10){
		targetType |= Blink;
	}
	else if(randSetup < 30){
		targetType |= Fast;
	}
	pT = types[(rand() % numOfTypeChances)];
	int x = (rand() % WINDOW_WIDTH);
	int y = (rand() % (WINDOW_HEIGHT-200));

	if(pT == Plus || pT == Minus){
		value = values[(rand() % addSubValues)];
	}
	else if(pT == Multi || pT == Divi){
		value = ((rand() % divMultiMax)+2);
	}
	target = new Target(targetType,pT,value,NULL);
	target->setPosition(x,y);
	target->setWidth(64);
	target->setHeight(64);
	Sprite2d* sprite = SpriteManager::instance()->createSprite(target,"target.png",0);
	target->setSprite(sprite);
	return target;
}

void ShootingGalleryGame::beginMainMenu(){
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	UiButton* playB = new UiButton(240,350,128,128,"PlayButtonD.png","PlayButtonS.png","PlayButtonH.png",
									FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,NULL,&changeGameState,(void*)GamePlay);
	m_GameObjects->push_back(playB);

	UiButton* highScoreButton = new UiButton(550,350,128,128,"HighScoreD.png","HighScoreS.png","HighScoreH.png",
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
		m_GameObjects->push_back(spawnTarget());
	}
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	m_TimeRemaining = 60;
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
	UiButton* backButton = new UiButton(100,100,128,128,"PlayButtonD.png","PlayButtonS.png",
									FIRE_ON_RELEASED,NULL,&changeGameState,(void*)MainMenu);
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
	Text* text = NULL;
	score->m_HighScore = m_Score;
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	UiButton* finishedB = new UiButton(800,200,64,128,"Finished.png","FinishedS.png",
									FIRE_ON_RELEASED,NULL,&saveHighScore,(void*)score);
	m_GameObjects->push_back(finishedB);
	Vector3 c(170,80,100);
	TextManager::instance()->createText("YourScore: ","tfa_squaresans.ttf",30,c,255,300,400,false,0,0);
	TextManager::instance()->createText(intToString(m_Score),"tfa_squaresans.ttf",30,c,255,555,400,false,0,0);
	//create three UiListMenu objects with there entries set to the letters of the alphabet
	UiListMenu* letter = NULL;
	UiButton* scrollUp;
	UiButton* scrollDown;
	for(int i = 0;i < 3;i++){
		letter = NULL;
		scrollUp = new UiButton(200+(i*200),50,128,128,"UpArrow.png","UpArrowS.png",
										FIRE_ON_RELEASED,NULL,&scrollUpLetterSelect,NULL);
		scrollDown = new UiButton(200+(i*200),250,128,128,"DownArrow.png","DownArrowS.png",
										FIRE_ON_RELEASED,NULL,&scrollDownLetterSelect,NULL);

		letter = new UiListMenu(200+(i*200),50,NULL,scrollUp,scrollDown,45,125,1,50);
		letter->setWidth(150);
		letter->setHeight(200);
		for(int j = 0;j < LETTERS_NUMBERS_COUNT;j++){
			text = TextManager::instance()->createText(LETTERS_NUMBERS[j],"tfa_squaresans.ttf",64,c,255,0,0,0,false,0);
			letter->addEntry(text);
		}
		switch(i){
		case 0:
			m_Letter1 = letter;
			break;
		case 1:
			m_Letter2 = letter;
			break;
		case 2:
			m_Letter3 = letter;
			break;
		}
	}
}
void ShootingGalleryGame::endNewHighScore(){
	TextManager::instance()->deleteAllText();
		SpriteManager::instance()->deleteSprite(m_Background);
	if(m_GameObjects){
		SafeVectorDelete<GameObject>(*m_GameObjects);
		m_GameObjects->clear();
	}
	SafePtrRelease(m_Letter1);
	SafePtrRelease(m_Letter2);
	SafePtrRelease(m_Letter3);
}
void ShootingGalleryGame::beginPlayAgain(){
	m_Background = SpriteManager::instance()->createSprite(NULL,"MainMenuBack.png",10);
	UiButton* playAgainB = new UiButton(300,300,64,128,"Finished.png","FinishedS.png",
									FIRE_ON_RELEASED,NULL,&changeGameState,(void*)GamePlay);
	m_GameObjects->push_back(playAgainB);
	UiButton* quitB = new UiButton(600,300,128,128,"quitD.png","quitS.png",
									FIRE_ON_RELEASED,NULL,&changeGameState,(void*)MainMenu);
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
	switch(event){
	case MOUSE_LB_RELEASED:{
			std::vector<Target*> targetsToRemove;
			Target* target;
			std::string hitDisplay;
			int addedScore = 0;
			for(int i = 0;i < m_GameObjects->size();i++){
				if((*m_GameObjects)[i]->getType() == "Target"){
					target = (Target*)(*m_GameObjects)[i];
					if(target->hit(x,y)){
						switch(target->getPointType()){
						case Plus:
						case Multi:
							hitDisplay += "+";
							break;
						default:
							break;
						}
						hitDisplay += intToString(target->calcNewScore(m_Score) - m_Score);
						Vector3 color = GREEN;
						if(hitDisplay[0] == '-'){
							color = RED;
						}
						TextManager::instance()->createText(hitDisplay,"tfa_squaresans.ttf",30,color,255,
							target->getPositionX(),target->getPositionY(),0,true,100);
						targetsToRemove.push_back(target);
						addedScore += (target->calcNewScore(m_Score) - m_Score);
					}//check hit.
				}//check target
			}//game object loop
			m_Score += addedScore;
			for(int i = 0;i < targetsToRemove.size();i++){
				for(int j = 0;j < m_GameObjects->size();j++){
					if(targetsToRemove[i] == (*m_GameObjects)[j]){
						SafePtrRelease((*m_GameObjects)[j]);
						m_GameObjects->erase(m_GameObjects->begin()+j);
						break;
					}
				}
			targetsToRemove.clear();
		}
		break;
	}
	default:
		break;
	}
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
					m_SetToSpawn = false;
				}
			}
		}
		break;
	default:
		break;
	}
}

void changeGameState(void* state){
	MessageHandler::Instance()->createMessage(CHANGE_STATE,ShootingGalleryGame::instance(),
												ShootingGalleryGame::instance(),state,0);
}
void saveHighScore(void* highScore){
	HighScore* score = (HighScore*)highScore;
	std::vector<HighScore>* scores = ShootingGalleryGame::instance()->getHighScores();
	score->m_Name = "";
	score->m_Name += ShootingGalleryGame::instance()->
						getLetter1()->getSelectedEntry()->getText();
	score->m_Name += ShootingGalleryGame::instance()->
						getLetter2()->getSelectedEntry()->getText();
	score->m_Name += ShootingGalleryGame::instance()->
						getLetter3()->getSelectedEntry()->getText();
	for(int i = 0;scores->size();i++){
		if((*scores)[i].m_HighScore <= score->m_HighScore){
			scores->insert(scores->begin()+i,*score);
			scores->pop_back();
			ShootingGalleryGame::instance()->setHighScores(scores);
			ShootingGalleryGame::instance()->saveHighScores();
			break;
		}
	}
	changeGameState((void*)HighScoreScreen);
}

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