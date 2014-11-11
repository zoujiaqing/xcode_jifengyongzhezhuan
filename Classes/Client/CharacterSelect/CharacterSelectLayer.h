#ifndef CHARACTER_SELECT_LEVEL_H
#define CHARACTER_SELECT_LEVEL_H


#include "cocos2d.h"
#include "UIEditBox.h"
#include "UIButton.h"
#include "UILayout.h"
#include "UIContainer.h"
#include "SpriteSeer.h"
#include "UILabel.h"
using namespace std;
using namespace cocos2d;
using namespace TXGUI;

class CharacterSelectLayer : public cocos2d::CCLayer
{
public:
	CharacterSelectLayer();
	virtual ~CharacterSelectLayer();

	CREATE_FUNC(CharacterSelectLayer);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
	void menuCreateRoleCallback(CCObject* pSender);
	void onItemClicked(CCObject* pSender);
	void setRandomName(CCObject* pSender);
private:

	void CreateCharacter();
	void ShowCharacterInfor(unsigned int index);
	void initCharacterButton();
	void ShowCharacterModle(unsigned int index);
	void ShowCharacterBigPicture(unsigned int index);
	void setOccupationName(const char* text);
	void setOccupationDes(const char* text);

	void onReceiveNickName(CCNode* node,void* nick);

	void showMessage(const char* msg);
	bool checkNullString(const char* str);
private:
	CCMenuItemImage *pLoginItem;
	UIEditBox* mTextField;
	unsigned int m_currentCharacterIndex;
	int m_characterButtonPosX;
	UIButton* m_currentButton;
	UILayout* m_layout;
	UIContainer* m_container;
	SpriteSeer* m_currentModle;
	UILabel* m_occupationLabel;
	UILabel* m_occupationDes;
	CCSprite* m_characterPic;
	CCPoint m_picPoint;
	std::string m_lastRandomName; // 记录随机名字，防止用户输入的名字在换职业时被替换
};

#endif