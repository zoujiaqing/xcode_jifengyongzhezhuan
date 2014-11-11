#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <map>
#include <stack>

#include "AspriteManager.h"
#include "UserData.h"

class LevelLayer;

//////////////////////////////////////////////////////////////////////////
struct sLevelData
{	
	unsigned char*		mapFilePointer;
	ASprite*			objectsSpriteFilePointer;

	//add more here
	int levelID;

	int row;	// height
	int col;	// width
	int cellWidth;
	int cellHeight;

	sLevelData():mapFilePointer(NULL),objectsSpriteFilePointer(NULL), levelID(0){}
};

//////////////////////////////////////////////////////////////////////////
struct sLevelPack
{
	sLevelData*			levelData;
	LevelLayer*			levelLayer;
};

//////////////////////////////////////////////////////////////////////////
struct sActiveLevelConfig
{
	int			fromType;				//for seer refresh position
	CCPoint     pos;
	//add more here
};

//////////////////////////////////////////////////////////////////////////
// 地图状态
typedef enum 
{
	EUninit   	 = 0,	// 登录状态
	EInHomeTown  = 1,	// 主城中
	EChangeMap   = 2,	// 地图切换中
	EInInstance  = 3,	// 副本中
}levelStatus;

typedef std::map<unsigned int , sLevelPack*	> LEVEL_MAP;

//////////////////////////////////////////////////////////////////////////

class LevelManager
{
public:

	LevelManager();
	~LevelManager();

	static LevelManager*		sShareInstance();
	static void Destroy();

	//////////////////////////////////////////////////////////////////////////
	//load/free level
	LevelLayer*					loadLevel(unsigned int levelId,bool bStoryMode = false);
	void						ShowMonsterOrNot(bool bShow);
	void						freeLevel(unsigned int levelId, bool freeData);
	void						freeLevel(LevelLayer* levelLayer, bool freeData);
	void						freeAllLevels(bool freeData);

	//////////////////////////////////////////////////////////////////////////
	//active/de-active level
	LevelLayer*					activeLevel(unsigned int levelId, sActiveLevelConfig& cofig,bool bStoryMode = false);	
	void						deactiveCurrentLevel(bool freeLevel = true, bool freeData = true);	

	/**
	* Instruction : 判断是否为主城
	* @param 
	*/
	/*bool						isMainLandLevel();*/
	int getCurMapId();

	/// check current level is ppve map level
	bool	isCurrentPPVELevel();
    /// check current level is pve map level
	bool	isCurrentPVELevel();
	bool	isCurrentPVPLevel();
	bool	isCurrentPVAILevel();
	bool	isCurrentEliteLevel();
	bool    isCurrentUndergroundLevel();
	bool    isCurrentCityDefendLevel();
	bool    isCurrentWorldBossLevel();
	bool    isCurrentBestFighterLevel();
	bool    isCurrentCommonPKLevel();
	bool    isCurrentMandPKLevel(); 
	bool    isCurrentHeroInstanceLevel();
	bool    isCurrentGuildInstanceLevel();


	//current active level
	LevelLayer*					getCurrentLevelLayer();
	CCPoint						pointCellToPixel(CCPoint pointCell);
	CCPoint						pointPixelToCell(CCPoint pointPixel);
	bool                        isPointReachable(CCPoint pointPixel);

	levelStatus					getCurrentLevelStatus(){return mLevelStatus;}
	void						setCurrentLevelStatus(levelStatus status){mLevelStatus = status;}

	void	SetBattleLevel(int levelID) {m_BattlelevelID = levelID;}

		/// check given map whether is ppve level or not
	bool	isPPVELevel(unsigned int levelID);

	bool	isPVELevel(unsigned int levelID);

	bool	isPVPLevel(unsigned int level);

	bool	isPVAILevel(unsigned int levelID);

	bool    isMandPKLevel(unsigned int level) ;

	bool    isCommonPKLevel(unsigned int level) ;

	bool    isEliteLevel(unsigned int levelID);

	bool	isUndergroundLevel(unsigned int levelID);

	bool	isCityDefendLevel(unsigned int levelID);

	bool	isWorldBossLevel(unsigned int levelID);

	bool	isBestFighterLevel(unsigned int levelID);

	bool	isHeroInstanceLevel(unsigned int levelID);

	bool	isGuildInstanceLevel(unsigned int levelID);

	void	addOnePlayerTypeToBattle(unsigned int roleType);
	const std::set<unsigned int>& GetPlayerTypes();
	void	ClearAllPlayerTypes();
protected:

	void						removeLevelFromParent(LevelLayer* levelLayer);

	//bool						isHomeTownId(unsigned int mapId);


public:

	struct OTHER_PLAYER_INFO
	{
		PLAYER_ID uid;
		std::string userName;
		unsigned int type;
		cocos2d::CCPoint pos;
		unsigned int orient;
		unsigned int battleSide;
	};

	std::stack<OTHER_PLAYER_INFO> m_otherPlayerInfo;

protected:

	//////////////////////////////////////////////////////////////////////////
	//level data manage
	sLevelData*					loadLevelData(unsigned int levelId);
	void						freeLevelData(unsigned int levelId);	

private:

	static LevelManager*		sInstance;

	LevelLayer*					mCurrentLevel;
	LEVEL_MAP					mLevelList;
	levelStatus					mLevelStatus;

	int m_BattlelevelID;

	std::set<unsigned int> battlePlayerTypes;
};

// ---------------------------------------------------------------------------------------

class Machtimer :public cocos2d::CCObject
{
private:
	long t0;
public:
	Machtimer(void){init();}

	void init();
	void start();
	long elapsed();
	// return elapsed of time in Sec Unit: 
	long elapsedSec();
	// return elasped of time in MS
	long elapsedMS();
	static void initialize();
	static long currentTimeInMS();
};


#endif