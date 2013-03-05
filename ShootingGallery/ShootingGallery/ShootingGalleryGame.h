#ifndef SHOOTING_GALLERY_GAME_H
#define SHOOTING_GALLERY_GAME_H
#include "game.h"
#include "inputListener.h"
#include "MessageListener.h"
#include <vector>
#include <string>

enum SGGameStates{
	None,
	MainMenu,
	GamePlay,
	HighScoreScreen,
	NewHighScore,
	PlayAgain
};

struct HighScore{
	std::string m_Name;
	int m_HighScore;
};

class GameObject;
class Target;
class Sprite2d;
class ShootingGalleryGame : public Game , public InputListener , public MessageListener{
public:
	static ShootingGalleryGame* instance();
	void cleanupInstance();
	void update();
	void reset();

	void saveHighScores();
	void loadHighScores();

	Target* spawnTarget();

	void changeState(const SGGameStates& state);
protected:
	ShootingGalleryGame();
	virtual ~ShootingGalleryGame();
	void loadContent();
	void initialize();
	static ShootingGalleryGame* m_Instance;

	void (ShootingGalleryGame::*SGUpdate)();

	void gamePlay();
	void mainMenu();
	void highScore();
	void newHighScore();
	void playAgain();

	void beginMainMenu();
	void endMainMenu();

	void beginGame();
	void endGame();

	void beginHighScore();
	void endHighScore();

	void beginNewHighScore();
	void endNewHighScore();

	void beginPlayAgain();
	void endPlayAgain();

	std::vector<GameObject*>* m_GameObjects;
	std::vector<HighScore*>* m_HighScores;
	Sprite2d* m_Background;
	int m_Score;
	SGGameStates m_CurrentState;
	bool m_Paused;

	void mouseInputCalback(const inputEvent& event,const int& x,const int& y);
	void handleMessage(const Message& msg);
};

void pause(void* param);//pause/unpause.
void changeGameState(void* state);
void saveHighScore(void* highScore);
#endif