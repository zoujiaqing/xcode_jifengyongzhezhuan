//////////////////////////////////////////////////////////////////////////
//new sprite map
//////////////////////////////////////////////////////////////////////////


#ifndef AStart_SpriteMap_h
#define AStart_SpriteMap_h

#include "ASprite.h"
#include "cocos2d.h"

//-----------------------------------------------------------------------

using namespace cocos2d;
using namespace std;


//-----------------------------------------------------------------------
typedef struct 
{
    int			w;		//权重
    int			step;	//步长
    int			state;	//状态
    int			x;		//坐标x
    int			y;		//坐标y
}QueData;

//-----------------------------------------------------------------------
enum
{
    ACTION_STANDBY  = 102,
    ACTION_FRONT    = 101,
    ACTION_BACK     = 100,
    ACTION_ALL      = 103
};

//-----------------------------------------------------------------------

enum  DIRECTION_ACTOR
{
	DIRECTION_UNKNOW = -1,
    DIRECTION_UP  = 0,
	DIRECTION_LEFT= 1,
	DIRECTION_BOTTOM = 2,
	DIRECTION_RIGHT  = 3,

	E_COUNT
};

enum  DIRECTION_MONSTER
{
	DIRECTION_MONSTER_UNKNOW = -1,
    DIRECTION_UP_LEFT  = 0,
	DIRECTION_UP_RIGHT= 1,
	DIRECTION_DOWN_LEFT = 2,
	DIRECTION_DOWN_RIGHT  = 3,
};

enum MONSTER_STATUS
{
	MONSTER_UNKNOWN = 0,

	MONSTER_IDLE = 1,
	MONSTER_RUN = 2,
	MONSTER_ATTACK = 3,
	MONSTER_HURT = 4,
	MONSTER_DEAD = 5
};

// the object 's movement type, move by the joystick or by the path finding 
enum Move_Type
{
	MOVE_BY_JOY = 0,
	MOVE_BY_MOUSE = 1,
};

//-----------------------------------------------------------------------

class CPhysicsLayer
{

public:

    CPhysicsLayer();
    ~CPhysicsLayer();

public:

    bool				init(BBYTE* file, int row, int col, int cellWidth, int cellHeight);
    bool				astarSearch();
    void				setStart(cocos2d::CCPoint pt);
    void				setEnd(cocos2d::CCPoint pt);
    int					getStart()									{return start;}
    int					getEnd()									{return	end;}
    int					getPathTop()								{return pathTop;}
    cocos2d::CCPoint	getPathPos(int step);
    //inline int			getCubeSize()								{return cubeSize;}
    CCPoint				getRandomPoint();
	bool				isPointReachable(CCPoint point);
	bool				isPointReachable(const cocos2d::CCRect& rect);
	int					getCellWidth(){return cubeSizeWidth;}
	int					getCellHeight(){return cubeSizeHeight;}

	cocos2d::CCPoint	pointCellToPixel(CCPoint pointCell);
	cocos2d::CCPoint	pointPixelToCell(CCPoint pointPixel);

private:

    void				readMapInfoFromPlist(BBYTE* file, int row, int col, int cellWidth, int cellHeight);
    void				push_heap(QueData a[],int pa);
    void				pop_heap(QueData a[],int pa);
    int					abs(int x);
    int					diff(int x,int y);
    bool				inside(int x,int y);
    void				getPath(int state);

	//-----------------------------------------------------------------------
	//			get the path navigation info from map file
	//-----------------------------------------------------------------------
	int					getMapNavigationInfo(int x, int y);
	


private:

	//-----------------------------------------------------------------------
	//			share the map navigation info
	//-----------------------------------------------------------------------
    unsigned char*		mapsNavInfo;


    int					start;//开始的位置
    int					end;//结束的位置
    int					goCount;
    int**				path;//path[ROW*COL][2]; //最终的路径
    int					pathTop;//最终路径点的数量
    int					d[8][2];
    int*				pre;//pre[ROW*COL];
    int*				best;//best[ROW*COL];
    QueData*			que;//que[ROW*COL];

    
    //物理层的行列大小， 注意，物理层默认是16*16， 地图的Tile默认是64*64
    int					row;
    int					col;
    int					cubeSizeWidth;
	int					cubeSizeHeight;
};


#endif
