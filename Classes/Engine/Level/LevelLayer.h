#ifndef LEVEL_LAYER_H
#define LEVEL_LAYER_H



#include <vector>
#include "Define.h"

#include "LevelManager.h"
//#include "CameraController.h"
#include "cocos2d.h"
#include "SpriteTranspointListener.h"
#include "SpriteTransPoint.h"
#include "SpriteMonster.h"

class SpriteSeer;
class SpriteElf;
class SpriteMine;
class BaseListener;
class BaseLevelBuilder;
class CPhysicsLayer;
class SpriteMonster;
class SpriteTransPoint;

//-----------------------------------------------------------------------

struct MAP_POINT_INFO
{
	MAP_POINT_INFO()
		: name(NULL)
		, ID(0)
	{

	}
	char *name;
	CCPoint point;
	int ID;
};

struct MAP_NPC_INFO : public MAP_POINT_INFO
{
	CCPoint patrolPos;
	CCSize patrolBound;
};



//-----------------------------------------------------------------------

typedef struct  
{
	SpriteSeer*		seer;
	SpriteElf*		pet;
#if _DEBUG
	std::string name;
#endif

}OthersPlayerInfo;


//-----------------------------------------------------------------------
enum MAP_Z
{
	MAP_Z_MIN = 0,
	MAP_Z_BACK1 = 1,	
	MAP_Z_TILE_BACKGROUND,
	MAP_Z_BG1,
	MAP_Z_TRANSPOINT,
	MAP_Z_BG2,
	MAP_Z_BG3,
	MAP_Z_ROLE_SHADOW,
	MAP_Z_BG4,
	MAP_Z_BG5,
	MAP_Z_BG6,
	MAP_Z_BG7,
	MAP_Z_BG8,
	MAP_Z_BG9,
	MAP_Z_OBJECT, 
	MAP_Z_OBJECT_TOUCH,
	MAP_Z_TOUCH_AINM,

	MAP_Z_DYNAMIC_BEGIN,
	MAP_Z_DYNAMIC = MAP_Z_DYNAMIC_BEGIN + 100000,
	MAP_Z_DYNAMIC_END = MAP_Z_DYNAMIC + 100000,

	MAP_Z_BACK2,        

	MAP_Z_PHYSICS,

	MAP_Z_OBJ_1,
	MAP_Z_OBJ_2,
	MAP_Z_OBJ_3,
	MAP_Z_OBJ_4,
	MAP_Z_OBJ_5,
	MAP_Z_OBJ_6,
	MAP_Z_OBJ_7,
	MAP_Z_OBJ_8,

	MAP_Z_OBJ_SHADOW,

	MAP_Z_OBJ_TOP,

	MAP_Z_LIGHT,

	MAP_Z_UI,



	// MUST BE LAST!!!
	MAP_Z_MAX,

};

//-----------------------------------------------------------------------

enum MAP_OBJECT_TAG
{
	//backs
	MAP_TAG_BACKGROUND0					= 100,
	MAP_TAG_BACKGROUND1					= 101,
	
	//seer
	MAP_TAG_SEER						= 102,
	MAP_TAG_OTHER_SEER					= 103,

	//follow sprite
	MAP_TAG_FOLLOW_SPRITE				= 104,
	MAP_TAG_OTHER_FOLLOW_SPRITE			= 105,

	//animations
	MAP_TAG_ANIMATIONS					= 106,	

	//room animation
	MAP_TAG_ROOM_ANIMATION				= 107,

	//mines
	MAP_TAG_MINE						= 108,	

	//transport
	MAP_TAG_TRANSPOINT					= 109,
	
	//wild monsters
	MAP_TAG_MONSTER						= 110,	

	// npc
	MAP_TAG_NPC							= 111,

	MAP_TAG_ELF							= 112,

	//add more here

};


//-----------------------------------------------------------------------
class LevelLayer : public cocos2d::CCLayer
{

	friend class LevelManager;
	friend class BaseLevelBuilder;

public:

	LevelLayer();
	virtual ~LevelLayer();	
	// implement the "static node()" method manually
	CREATE_FUNC(LevelLayer);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

public:
	
	static int						sCalcZorder(CCPoint pos);
	static float					sCalcDepth(unsigned int z);
	static BaseListener*			sGetListenerByTag(int tag);

	
	//-----------------------------------------------------------------------
	//	return only your own seer and follow seer not other players'
	//-----------------------------------------------------------------------
	SpriteSeer*						getSpriteSeer();
	SpriteElf*						getFollowSprite();
	int								getObjectCount(int tag);
	CCPoint							getRandomPoint();
	CCPoint							getSeerBornPoint(sActiveLevelConfig& config);
	int								getID();

	CPhysicsLayer*					getPhysicsLayer(void){return m_pPhysicsLayer;}
	sLevelData*						GetData(void){return m_levelDataReference;}

	CCSize							getMapSizeInPixel(void);
	CCSize							getMapTileSizeInPixel( void );
	CCRect							getMapRect(void);

	CCNode*							getObjectLayer(void){return m_pObjectsLayer;}

	SpriteTransPoint*				getMainLandTranspoint();
	//get jing ying fu ben chuan shong dian
	SpriteTransPoint* getEliteTranspoint();


	//-----------------------------------------------------------------------
	//	add/remove/update other player
	//-----------------------------------------------------------------------
	void		addOtherPlayer(PLAYER_ID uid, const char* userName, unsigned int type, cocos2d::CCPoint pt, unsigned int orient, unsigned int battleSide);
	void		removeOtherPlayer(PLAYER_ID uid);
	void        removeAllOtherPlayer();
	void		updateOtherPlayer(PLAYER_ID uid, cocos2d::CCPoint pt, unsigned int animID, bool bFlip);
	void		DoOtherPlayerSkill(PLAYER_ID uid, unsigned int skillID, const CCPoint& rolePos, const CCPoint& roleDir, const CCPoint& aimDir, const CCPoint& monsterPos, bool bAnimMonster);
	void		DoElfSkill(PLAYER_ID uid, unsigned int skillID, const CCPoint& rolePos, const CCPoint& roleDir, const CCPoint& aimDir, const CCPoint& monsterPos, bool bAnimMonster,unsigned int pos = 0);

	SpriteSeer* getPlayer(PLAYER_ID uid);
	
	// Note: 
	void	EquipOnePlayerWeapon(int playerId,int weaponId);
	
	///  add monster / delete monster
	void	addMonster(MonsterInitInfo info);
	void    removeMonster(unsigned int uid);

	void	ShowMonsterOrNot(bool bShow);

	void    showTouchEffect(CCPoint point);

public:

	virtual void					update(float dt);
	virtual void					ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);	
	virtual void					ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void					ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	virtual void					visit();

public:
	struct WAKL_INFO
	{
		struct WAKL_PACK
		{
			WAKL_PACK():bUsed(false),animID(-1), bFlip(false){uid.setPlayId(0,0,0);}
			PLAYER_ID uid;
			cocos2d::CCPoint pt;
			unsigned int animID;
			bool bFlip;
			bool bUsed;
		};

		struct IDLE_TIME
		{
			float time;
			float curTime;
		};
		
		WAKL_PACK walk;
		IDLE_TIME idleTime;
	};

	std::map<PLAYER_ID , WAKL_INFO> m_walkInfo;

	//---------------------------------------------------------------------------------
//				other players record
	//---------------------------------------------------------------------------------	
	std::map<PLAYER_ID, OthersPlayerInfo>	m_othersPlayerInfo;

protected:
	
	//-----------------------------------------------------------------------
	static LevelLayer*				node(unsigned int levelId, sLevelData* data,bool bStoryMode = false);
	bool							initLevel(unsigned int levelId, sLevelData* data,bool bStoryMode = false);
	void							onlyCreateMonsters();
	void							destroyLayer();

	//-----------------------------------------------------------------------
	void							onActiveLayer(sActiveLevelConfig& config);
	void							onDeactiveLayer();	
	
	//-----------------------------------------------------------------------	
	void							handleMapScrollEvent(CCPoint winPT);

protected:

	//void							parseData(sLevelData* data);	
	
	void							debugDrawPath();

protected:

	std::vector<CCNode*>			getChildrenByTag(int tag); 

private:	

	unsigned int					m_id;
	
	//---------------------------------------------------------------------------------
	//				map config
	//---------------------------------------------------------------------------------	
	int								m_pathOffset;
	std::vector<MAP_POINT_INFO>		m_mineInfoList;
	std::vector<MAP_POINT_INFO>		m_bornPointList;
	CPhysicsLayer*					m_pPhysicsLayer;


	
	//---------------------------------------------------------------------------------
	//				other monster record
	//---------------------------------------------------------------------------------	
	std::map<uint32_t, SpriteMonster*>	m_MonsterInfo;

	//---------------------------------------------------------------------------------
	//				data reference
	//---------------------------------------------------------------------------------	
	sLevelData*						m_levelDataReference;	

	
	//---------------------------------------------------------------------------------
	//				level builder
	//---------------------------------------------------------------------------------	
	BaseLevelBuilder*				m_levelBuilder;


	//---------------------------------------------------------------------------------
	//				touch point record used by scroll event control
	//---------------------------------------------------------------------------------	
	CCPoint							m_touchWinPoint;

	CCNode*							m_pObjectsLayer;

	//CameraController m_cameraController;
	
	float pressTime;

	EffectSprite * touchEffect;
	
};

#endif
