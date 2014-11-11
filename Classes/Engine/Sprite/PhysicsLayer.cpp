//-----------------------------------------------------------------------
//					CPhysicsLayer.cpp
//					AStart
//-----------------------------------------------------------------------


#include "PhysicsLayer.h"
#include "CrossPlatform.h"

#include "Define.h"
#include "UIDefine.h"
#include "LevelDefine.h"


using namespace cocos2d;



CPhysicsLayer::CPhysicsLayer()
	:mapsNavInfo(NULL)
	,row(0)
	,col(0)
	,cubeSizeWidth(0)
	,cubeSizeHeight(0)
	,path(NULL)
	,pre(NULL)
	,best(NULL)
	,que(NULL)
{
}


CPhysicsLayer::~CPhysicsLayer()
{   
    
    for (int i=0; i< col * row; i++)
    {
        free(path[i]);
    }
    free(path);

    free(pre);
    free(best);
    free(que);
	delete[] mapsNavInfo;
	mapsNavInfo = NULL;
}


bool CPhysicsLayer::init(BBYTE* file, int row, int col, int cellWidth, int cellHeight)
{
    goCount=0;
    pathTop=0;
    start = end = -1;
    d[0][0]=-1;
    d[0][1]=0;
    d[1][0]=0;
    d[1][1]=1;
    d[2][0]=1;
    d[2][1]=0;
    d[3][0]=0;
    d[3][1]=-1;
    d[4][0]=-1;
    d[4][1]=-1;
    d[5][0]=-1;
    d[5][1]=1;
    d[6][0]=1;
    d[6][1]=1;
    d[7][0]=1;
    d[7][1]=-1;
    
    
    
    readMapInfoFromPlist(file, row, col, cellWidth, cellHeight);
    return true;
}

//-----------------------------------------------------------------------
// 读取地图信息的配置表
//-----------------------------------------------------------------------
void CPhysicsLayer::readMapInfoFromPlist(BBYTE* file, int row, int col, int cellWidth, int cellHeight)
{    
	this->row = row;	
	this->col = col;	
	cubeSizeWidth = cellWidth;
	cubeSizeHeight = cellHeight;

	int count = row * col;
	mapsNavInfo = new unsigned char[count];
	for(int y = 0; y < row; y++)
	{
		for(int x = 0; x < col; x++)
		{
			mapsNavInfo[x + y * col] = file[y + x * row] & 0x01;
		}
	}

    path = (int **)malloc(sizeof(int*) * row * col);
    for (int i=0; i< col * row; i++)
    {
        path[i] = (int*)malloc(sizeof(int) * 2);
    }
    
    pre  = (int*)malloc(sizeof(int) * row * col);
    best = (int*)malloc(sizeof(int) * row * col);
    
    que = (QueData*)malloc(sizeof(QueData) * col * row);   
}


int CPhysicsLayer::getMapNavigationInfo(int x, int y)
{
	return (int) (*((mapsNavInfo) + x * col + y));
}



void CPhysicsLayer::setStart(CCPoint pt)
{
	int curRow = (int)pt.y / cubeSizeHeight;
	int curColoum = (int)pt.x / cubeSizeWidth;
	curRow = clampf(curRow, 0, row - 1);
	curColoum = clampf(curColoum, 0, col -1);

    start = curRow * col + curColoum;
}
void CPhysicsLayer::setEnd(CCPoint pt)
{
	int curRow = (int)pt.y / cubeSizeHeight;
	int curColoum = (int)pt.x / cubeSizeWidth;
	curRow = clampf(curRow, 0, row - 1);
	curColoum = clampf(curColoum, 0, col -1);

	end = curRow * col + curColoum;
}

//#pragma mark AStarSearch
int CPhysicsLayer::abs(int x)
{
	return x >= 0 ? x : -x;
}

int CPhysicsLayer::diff(int x,int y)
{
	return abs(x - end / col) + abs(y - end % col);
}

bool CPhysicsLayer::inside(int x,int y)
{
	return x >= 0 && x < row && y >= 0 && y < col;
}

void CPhysicsLayer::push_heap(QueData a[],int pa)
{
	int pos=pa-1;
	QueData tmp=a[pa-1];
	while(pos > 0 && ( a[(pos-1)>>1].w > tmp.w  ||  (a[(pos-1)>>1].w==tmp.w  &&  a[(pos-1)>>1].step>tmp.step)))
	{
		a[pos]=a[(pos-1)>>1];
		pos=(pos-1)>>1;
	}
	a[pos]=tmp;
}
void CPhysicsLayer::pop_heap(QueData a[],int pa)
{
	int pos=1;
	QueData tmp=a[pa-1];
	if(pos+1<pa-1&&(a[pos].w>a[pos+1].w  ||  (a[pos].w==a[pos+1].w  &&  a[pos].step>a[pos+1].step)))
	{
		pos++;
	}
	while(pos<pa-1  &&  (tmp.w>a[pos].w  ||  (tmp.w==a[pos].w  &&  tmp.step>a[pos].step)))
	{
		a[(pos-1)>>1]=a[pos];
		pos=(pos<<1)+1;
		if(pos+1<pa-1  &&  (a[pos].w>a[pos+1].w  || (a[pos].w==a[pos+1].w  &&  a[pos].step>a[pos+1].step)))
		{
			pos++;
		}
	}
	a[(pos-1)>>1]=tmp;
}


cocos2d::CCPoint CPhysicsLayer::getPathPos(int step)
{
	return  pointCellToPixel(ccp(path[step][1], path[step][0]));
}


void CPhysicsLayer::getPath(int state)
{
	if(state == start)
	{
		path[pathTop][0] = state / col;
		path[pathTop++][1] = state % col;
		return;
	}
	getPath(pre[state]);
//	printf("%d,%d\n",state / col, state % col);
	path[pathTop][0] = state / col;
	path[pathTop++][1] = state % col;
}


bool CPhysicsLayer::astarSearch()
{
    for (int i=0; i < row * col; i++)
	{
        memset(path[i], 0, sizeof(int) * 2);
    }

    memset(pre, 0, sizeof(int) * col * row);
    memset(que, 0, sizeof(QueData) * col * row);
    memset(best,-1,sizeof(int) * col * row);
    goCount = 0;
    pathTop = 0;    
	
	que[0].step = 0;
	que[0].state = start;
	que[0].w = 0;
	que[0].x = start / col;
	que[0].y = start % col;
	best[start] = 0;

	QueData now,next;
	int ph = 1;
	int i;
	bool cantTurn[4];

	while(ph)
	{
		goCount++;
		now = que[0];
		pop_heap(que,ph--);

		if(now.state == end)
		{
			getPath(now.state);
//			putchar('\n');
			return true;
		}

		if (best[now.state] != now.step)
		{
			continue;
		}

		memset(cantTurn,0,sizeof(cantTurn));

		for(i = 0;i < 8;i++)
		{
			if (i >= 4 && cantTurn[i-4])
				continue;

			next.x = now.x + d[i][0];
			next.y = now.y + d[i][1];
			if(inside(next.x,next.y) && getMapNavigationInfo(next.x, next.y) == 0) 
			{
				next.state = next.x * col + next.y;
				next.step = now.step + 10;
				if (i >= 4)
				{
					next.step += 4;
				}
				if(best[next.state] != -1 && best[next.state] <= next.step)
				{
					continue;
				}
				best[next.state] = next.step;
				next.w = next.step + 10 * diff(next.x,next.y);
				que[ph++] = next;
				push_heap(que,ph);
				pre[next.state] = now.state;
			}
			else if	(i < 4)
			{
				cantTurn[i] = true;
				cantTurn[(i+1)%4] = true;
			}
		}
	}
	return false;
}

CCPoint CPhysicsLayer::getRandomPoint()
{    
    int w = col;	
    int h = row;	
    
    while (1) 
	{
        int x = arc4random() % w;
        int y = arc4random() % h;

        if(getMapNavigationInfo(y, x) == 0)
        {    
			
              return  pointCellToPixel(CCPointMake( x, y));   
			
        }
    }

    return CCPointMake(0,0);
}

bool CPhysicsLayer::isPointReachable(CCPoint point)
{
	int curRow = (int)point.y / cubeSizeHeight;
	int curColoum = (int)point.x / cubeSizeWidth;
	if (curRow <= 0 || curColoum <= 0 || curRow > row -1 || curColoum > col - 1)
		return false;	

	return getMapNavigationInfo(curRow, curColoum) == 0;
}

bool CPhysicsLayer::isPointReachable(const cocos2d::CCRect& rect)
{
	// TODDO : 优化
	int left = rect.origin.x / cubeSizeWidth;
	
	if(left < 0)
		left = 0;
	if(left > col - 1)
		left = col - 1;

	int width = rect.size.width / cubeSizeWidth;
	int right = left + width;

	if(right < 0)
		right = 0;
	if( right > col - 1)
		right = col - 1;

	int bottom = rect.origin.y / cubeSizeHeight;

	if(bottom < 0)
		bottom = 0;
	if( bottom > row - 1)
		bottom = row - 1;

	for(int i = left; i <= right; i++)
	{
		if(getMapNavigationInfo(bottom, i) == 1)
			return false;
	}

	int height = rect.size.height / cubeSizeHeight;
	int top = bottom + height;

	if(top < 0)
		top = 0;
	if( top > row - 1)
		top = row - 1;

	for(int i = left; i <= right; i++)
	{
		if(getMapNavigationInfo(top, i) == 1)
			return false;
	}

	for(int i = bottom - 1; i < top; i++)
	{
		if(getMapNavigationInfo(i, left) == 1)
			return false;

		if(getMapNavigationInfo(i, right) == 1)
			return false;
	}

	return true;
}

cocos2d::CCPoint CPhysicsLayer::pointCellToPixel( CCPoint pointCell )
{
	if (pointCell.x == 0 && pointCell.y == 0)
	{
		return pointCell;
	}
	CCPoint pixPoint;
	
	pixPoint.x = pointCell.x * cubeSizeWidth + cubeSizeWidth / 2;
	pixPoint.y = pointCell.y * cubeSizeHeight + cubeSizeHeight / 2;

	return pixPoint;
}

cocos2d::CCPoint CPhysicsLayer::pointPixelToCell( CCPoint pointPixel )
{
	CCPoint cellPoint;

	cellPoint.x = (int)(pointPixel.x / cubeSizeWidth);
	cellPoint.y = (int)(pointPixel.y / cubeSizeHeight);

	return cellPoint;
}
