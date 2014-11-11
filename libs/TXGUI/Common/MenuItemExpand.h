#ifndef __MENU_ITEM__EXPAND_H__
#define __MENU_ITEM__EXPAND_H__

#include "cocos2d.h"
USING_NS_CC;

typedef enum{
	MENU_ITEM_AUTO = 0,
	MENU_ITEM_SELECT = 1,
	MENU_ITEM_UNSELECT = 2,
	MENU_ITEM_DISABLE = 3,
}MenuItemSpriteControyType;

class MenuItemExpand : public CCMenuItemSprite
{
public:
	MenuItemExpand();

	virtual void setEnabled(bool bEnabled);
	virtual void setMenuItemDispalyState(int state);

	virtual void selected();
	virtual void unselected();

	/** creates a menu item with a normal, selected and disabled image*/
	static MenuItemExpand * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite = NULL);
	/** creates a menu item with a normal and selected image with target/selector */
	static MenuItemExpand * create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector);
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	static MenuItemExpand * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);
protected:
	virtual void updateImagesVisibility();
private:
	MenuItemSpriteControyType mMenuItemSpriteControlType;
};

#endif // __MENU_ITEM__EXPAND_H__
