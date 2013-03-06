#include "Target.h"
#include "common.h"
#include "Util.h"
#include "ShootingGameConstants.h"
#include "MessageHandler.h"
#include "2dSprite.h"
#include "Vector3.h"
#include "Text.h"
#include "TextManager.h"

Target::Target(const unsigned int& type,
	const PointType& pType,const int& value,Sprite2d* sprite) :
GameObject(),
m_TargetType(type),
m_Type(pType),
m_Value(value),
m_Invis(false){
	m_Sprite = sprite;
	if(ContainsFlags(m_TargetType,Blink)){
		MessageHandler::Instance()->createMessage(SWITCH_VISIBILTY,this,this,NULL,INVIS_TIME);
	}
	std::string text;
	Vector3 color;
	switch(m_Type){
	case Plus:
		text += "+";
		color = GREEN;
		break;
	case Minus:
		text += "-";
		color = RED;
		break;
	case Multi:
		text += "*";
		color = GREEN;
		break;
	case Divi:
		text += "/";
		color = RED;
		break;
	default:
		break;
	}
	text += intToString(m_Value);
	
	m_Text = TextManager::instance()->createText(text,"tfa_squaresans.ttf",30,color,
		255,m_PositionX,m_PositionY,0,false,0);
}
Target::~Target(){
	MessageHandler::Instance()->deleteAllMessagesTo(this);
	TextManager::instance()->deleteText(m_Text);
}

void Target::update(){
	if(ContainsFlags(m_TargetType,StraightLeft)){
		if(ContainsFlags(m_TargetType,Fast)){
			m_PositionX -= (Util::instance()->getDelta()*FAST_SPEED);
		}
		else{
			m_PositionX -= (Util::instance()->getDelta()*NORMAL_SPEED);
		}
	}
	if(ContainsFlags(m_TargetType,StraightRight)){
		if(ContainsFlags(m_TargetType,Fast)){
			m_PositionX += (Util::instance()->getDelta()*FAST_SPEED);
		}
		else{
			m_PositionX += (Util::instance()->getDelta()*NORMAL_SPEED);
		}
	}
	if(ContainsFlags(m_TargetType,StraightUp)){
		if(ContainsFlags(m_TargetType,Fast)){
			m_PositionY -= (Util::instance()->getDelta()*FAST_SPEED);
		}
		else{
			m_PositionY -= (Util::instance()->getDelta()*NORMAL_SPEED);
		}
	}
	if(ContainsFlags(m_TargetType,StraightDown)){
		if(ContainsFlags(m_TargetType,Fast)){
			m_PositionY += (Util::instance()->getDelta()*FAST_SPEED);
		}
		else{
			m_PositionY += (Util::instance()->getDelta()*NORMAL_SPEED);
		}
	}
	m_Text->setPosition(m_PositionX,m_PositionY);
}

unsigned int Target::getTargetType() const{return m_TargetType;}
PointType Target::getPointType() const{return m_Type;}
int Target::getValue() const{return m_Value;}
bool Target::getInvis() const{return m_Invis;}
std::string Target::getType(){return "Target";}

bool Target::hit(const int& x,const int& y) const{
	if(x > m_PositionX && x < (m_PositionX+m_Width) &&
		y > m_PositionY && y < (m_PositionY+m_Height) && !m_Invis){
			return true;
	}
	return false;
}
int Target::calcNewScore(const int& score) const{
	if(m_Type == Plus){
		return (score+m_Value);
	}
	else if(m_Type == Minus){
		return (score-m_Value);
	}
	else if(m_Type == Multi){
		return (score*m_Value);
	}
	else if(m_Type == Divi){
		return (score/m_Value);
	}
}

void Target::handleMessage(const Message& msg){
	switch(msg.type){
	case SWITCH_VISIBILTY:{
			MessageHandler::Instance()->createMessage(SWITCH_VISIBILTY,this,this,NULL,INVIS_TIME);
			int alpha = m_Sprite->getColorApha();
			if(alpha == INVIS_ALPHA){
				m_Invis = false;
				m_Sprite->setColor(m_Sprite->getColor());
			}
			else{
				m_Invis = true;
				m_Sprite->setColor(m_Sprite->getColor(),INVIS_ALPHA);
			}
		}
		break;
	default:
		break;
	}
}