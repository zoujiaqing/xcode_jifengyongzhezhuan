#pragma  once
#include "Singleton_t.h"
#include "cocos2d.h"
#include "SceneLayer.h"


USING_NS_CC;

class SpriteSeer;
class ILogicManager;

// TODO : will remove this class.
class GameManager : public TSingleton<GameManager>
{
public :
	GameManager();
	virtual ~GameManager();

	void SetLogicManager(ILogicManager *pILogicMgr);

	SceneLayer* GetSceneLayer();

	SpriteSeer *getHero();

	void setHero(SpriteSeer* hero);

	int getHeroTypeId();
	void setHeroTypeId(int typeId);

	void processTouchBegan(CCPoint touchPoint);

	CCScene* goToSplash();

protected:
	ILogicManager* m_pLogicMgr;
	SpriteSeer* m_pSpriteSeer;
	int m_spriteSeerTypeId;
};