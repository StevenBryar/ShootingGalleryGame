#ifndef TARGET_H
#define TARGET_H
#include "gameObject.h"
#include "MessageListener.h"

enum TargetType{
	Blink = (1 << 0),
	StraightLeft = (1 << 1),
	StraightRight = (1 << 2),
	StraightUp = (1 << 3),
	StraightDown = (1 << 4),
	Fast = (1 << 5)
};
enum PointType{
	Plus = 1,
	Minus = 2,
	Multi = 3,
	Divi = 4
};

class Text;

class Target : public GameObject , public MessageListener{
public:
	Target(const unsigned int& type,const PointType& pType,const int& value,Sprite2d* sprite);
	virtual ~Target();

	void update();
	bool hit(const int& x,const int& y) const;
	int calcNewScore(const int& score) const;

	unsigned int getTargetType() const;
	PointType getPointType() const;
	int getValue() const;
	bool getInvis() const;
	std::string getType();
protected:
	unsigned int m_TargetType;
	PointType m_Type;
	int m_Value;
	bool m_Invis;
	Text* m_Text;

	void handleMessage(const Message& msg);
};
#endif