#ifndef SHOOTING_GALLERY_GAME_H
#define SHOOTING_GALLERY_GAME_H
#include "game.h"
#include "inputListener.h"

class GameObject;
class ShootingGalleryGame : public Game /*, public InputListener*/{
public:
	static ShootingGalleryGame* instance();
	void cleanupInstance();
	void update();
	void reset();
protected:
	ShootingGalleryGame();
	virtual ~ShootingGalleryGame();
	void loadContent();
	void initialize();
	static ShootingGalleryGame* m_Instance;
};
#endif