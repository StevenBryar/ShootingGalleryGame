#ifndef UI_LIST_MENU_H
#define UI_LIST_MENU_H
#include "inputListener.h"
#include "gameObject.h"
#include "uiElement.h"
#include "Vector3.h"
#include <map>

class Text;
class UiButton;

class UiListMenu : public UiElement , public InputListener{
public:
	UiListMenu(const int& screenPosX,const int& screenPosY,Camera* camera,
				UiButton* scrollUp,UiButton* scrollDown,const int& textOffsetX = 0,
					const int& textOffsetY = 0,const int& maxVisibleEntries = 3,int entryHeight = 30);
	virtual ~UiListMenu();

	void addEntry(Text* entry);
	bool deleteEntry(Text* entry);
	void clearEntries();

	void setOffsetX(const int& x);
	void setOffsetY(const int& y);
	void setUpButton(UiButton* up);
	void setDownButton(UiButton* down);
	void setMaxVisibleEntries(const int& max);
	void setEntries(std::map<int,Text*>* entry);
	void setSelectedEntry(const int& selected);
	void scrollUp();
	void scrollDown();
	void setDefaultTextColor(const Vector3& color);
	void setSelectedTextColor(const Vector3& color);

	UiButton* getUpButton() const;
	UiButton* getDownButton() const;
	int getTextOffsetX() const;
	int getTextOffsetY() const;
	int getMaxVisibleEntries() const;
	std::map<int,Text*>* getEntries() const;
	Text* getSelectedEntry() const;
	Text* getEntry(const int& p) const;
	Vector3 getDefaultTextColor() const;
	Vector3 getSelectedTextColor() const;

	void updateMenu();
	void update();

	void mouseInputCalback(const inputEvent& event,const int& x,const int& y);
protected:
	int m_TextOffsetX;
	int m_TextOffsetY;
	int m_MaxEntriesVisible;
	int m_Selected;
	int m_StartOfVisibles;
	int m_EntryHeight;
	Vector3 m_DefaultColor;
	Vector3 m_SelectedColor;
	std::map<int,Text*>* m_Entries;
	UiButton* m_Up;
	UiButton* m_Down;
};
#endif