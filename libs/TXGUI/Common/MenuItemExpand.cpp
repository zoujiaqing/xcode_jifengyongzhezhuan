#include "MenuItemExpand.h"


MenuItemExpand::MenuItemExpand():CCMenuItemSprite()
{
	mMenuItemSpriteControlType = MENU_ITEM_AUTO;
}

void MenuItemExpand::setEnabled(bool bEnabled)
{
	return CCMenuItemSprite::setEnabled(bEnabled);
}

void MenuItemExpand::updateImagesVisibility()
{
	if (mMenuItemSpriteControlType == MENU_ITEM_AUTO)
	{
		return CCMenuItemSprite::updateImagesVisibility();
	}
	else
	{
		if (m_pNormalImage)   m_pNormalImage->setVisible(false);
		if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
		if (m_pDisabledImage) m_pDisabledImage->setVisible(false);

		switch(mMenuItemSpriteControlType)
		{
		case MENU_ITEM_SELECT:
			{
				if (m_pSelectedImage) m_pSelectedImage->setVisible(true);
				break;
			}
		case MENU_ITEM_UNSELECT:
			{
				if (m_pNormalImage)   m_pNormalImage->setVisible(true);
				break;
			}
		case MENU_ITEM_DISABLE:
			{
				if (m_pDisabledImage) m_pDisabledImage->setVisible(true);
				break;
			}
		};
	}
}

void MenuItemExpand::setMenuItemDispalyState(int state)
{
	mMenuItemSpriteControlType = (MenuItemSpriteControyType)state;
	updateImagesVisibility();
}

MenuItemExpand * MenuItemExpand::create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite)
{
	return MenuItemExpand::create(normalSprite, selectedSprite, disabledSprite, NULL, NULL);
}

MenuItemExpand * MenuItemExpand::create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector)
{
	return MenuItemExpand::create(normalSprite, selectedSprite, NULL, target, selector);
}

MenuItemExpand * MenuItemExpand::create(CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector)
{
	MenuItemExpand *pRet = new MenuItemExpand();
	pRet->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector); 
	pRet->autorelease();
	return pRet;
}

void MenuItemExpand::selected()
{
	if (mMenuItemSpriteControlType == MENU_ITEM_AUTO)
	{
		return CCMenuItemSprite::selected();
	}
	else
	{
		CCMenuItem::selected();
	}	
}

void MenuItemExpand::unselected()
{
	if (mMenuItemSpriteControlType == MENU_ITEM_AUTO)
	{
		return CCMenuItemSprite::unselected();
	}
	else
	{
		CCMenuItem::unselected();
	}	
}