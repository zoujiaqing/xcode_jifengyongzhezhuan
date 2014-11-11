/**
 *============================================================
 *  @file      astar.hpp
 *  @brief    A* path finder \n
 *               see http://10.1.1.5/libtaomee++/pathfinder for examples
 * 
 *  compiler   gcc4.1.2
 *  platform   Linux
 *
 *  copyright:  TaoMee, Inc. ShangHai CN. All rights reserved.
 *
 *============================================================
 */

#ifndef LIBTAOMEEPP_TILEWORLD_HPP_
#define LIBTAOMEEPP_TILEWORLD_HPP_

/*
extern "C" {
#include <libtaomee/log.h>
}*/

#include <cstdlib>

#include <stdint.h>
#include <algorithm>
#include <fstream>
#include <list>
#include <math.h>

#include "def_grid_data.hpp"
#include "../base/ProbabilityGen.hpp"
#include "GameResourceManager.h"

using namespace cocos2d;


namespace taomee {

/**
 * @brief A* algorithm for path finding
 */
template <typename GridData = DefGridData>
class TileWorld {
private:
	/**
	  * @brief constants
	  */
	enum astar_constant_t {
		astar_grid_dist_strl	= 10,
		astar_grid_dist_oblique	= 14,
	};

	class Grid;
	/**
	  * @brief pointer to Grid
	  */
	typedef Grid* GridPointer;
	
public:
	/**
	  * @brief Point
	  */
	class Point {
	public:
		/**
		  * @brief default constructor
		  */
		Point(int32_t xpos = 0, int32_t ypos = 0)
			{ x = xpos; y = ypos; }
		
		float distance(const Point& rhs) const {
			return sqrt(distance2(rhs));
		}

		float distance2(const Point& rhs) const {
			return (x-rhs.x)*(x-rhs.x) + (y-rhs.y)*(y-rhs.y);
		}

		Point& operator+=(const Point& rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Point operator+(const Point& rhs) {
			Point r;
			r.x = x + rhs.x;
			r.y = y + rhs.y;
			return r;
		}

		Point& operator-=(const Point& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		
		Point operator-(const Point& rhs) {
			Point r;
			r.x = x - rhs.x;
			r.y = y - rhs.y;
			return r;
		}
		
		Point& operator*=(float multi) {
			x *= multi;
			y *= multi;
			return *this;
		}

		void unitised() {
			if(x > 0) x=1;
			if(x < 0) x=-1;
			if(y > 0) y=1;
			if(y < 0) y=-1;
		}
		
		Point& direct(const Point& rhs) {
			
		}

		bool operator==(const Point& rhs) const {
			return x == rhs.x && y == rhs.y;
		}

		void counterclockwise90(const Point& rhs) {
			int32_t tx = x;
			int32_t ty = y;
			x = -(ty - rhs.y) + rhs.x;
			y = tx - rhs.x + rhs.y;
		}

		void clockwise90(const Point& rhs) {
			int32_t tx = x;
			int32_t ty = y;
			x = (ty - rhs.y) + rhs.x;
			y = -(tx - rhs.x) + rhs.y;
		}

	public:
		/*! coordinate x */
		int32_t x;
		/*! coordinate y */
		int32_t y;
	};

	class Rectangle {
	public:
		Rectangle(Point start=Point(), Point end=Point()):
			start(start) , end(end) {
		}
		bool point_in_range(const Point& point) const {
			return ((point.x-start.x)*(point.x-end.x) <=0) && ((point.y-start.y) *(point.y-end.y) <=0 );
		}

		bool touch(const Rectangle& rec) {
			return point_in_range(rec.start) || point_in_range(rec.end) || point_in_range(Point(rec.start.x, rec.end.y)) 
			|| point_in_range(Point(rec.end.x, rec.start.y)) || rec.point_in_range(start) || rec.point_in_range(end)
			|| rec.point_in_range(Point(start.x, end.y)) || rec.point_in_range(Point(end.x, start.y))  ;
		}

		Point random_point() {
			return Point(base::ranged_random(start.x, end.x), base::ranged_random(start.y, end.y));
		}

		Point center() {
			return Point((start.x+end.x)/2, (start.y+end.y)/2);
		}

		uint32_t distance2(Rectangle rec) {
			return center().distance2(rec.center());
		}
		
		uint32_t distance(Rectangle rec) {
			return center().distance(rec.center());
		}

		Point nearest(const Point& point) const {
			Point ret;
			if(point.x >= start.x && point.x >= end.x)
			{
				ret.x = start.x > end.x? start.x:end.x;
			} else if(point.x <= start.x && point.x <= end.x)
			{
				ret.x = start.x < end.x? start.x:end.x;
			} else {
				ret.x = point.x;
			};

			if(point.y >= start.y && point.y >= end.y)
			{
				ret.y = start.y > end.y? start.y:end.y;
			} else if(point.y <= start.y && point.y <= end.y)
			{
				ret.y = start.y < end.y? start.y:end.y;
			} else {
				ret.y = point.y;
			};
			return ret;
		}

		Rectangle& move(const Point& direction) {
			start +=direction;
			end += direction;
			return *this;
		}
		
		Rectangle operator+(const Point& direction) {
			Rectangle rec = *this;
			rec.move(direction);
			return rec;
		}
		
		void counterclockwise90(const Point& rhs) {
			start.counterclockwise90(rhs);
			end.counterclockwise90(rhs);
		}
		
		void clockwise90(const Point& rhs) {
			start.clockwise90(rhs);
			end.clockwise90(rhs);
		}

		Rectangle mirrorY() {
			Rectangle rec = *this;
			rec.start.x = -rec.start.x;
			rec.end.x = -rec.end.x;
			return rec;
		}

		Rectangle mirrorX() {
			Rectangle rec = *this;
			rec.start.y = -rec.start.y;
			rec.end.y = -rec.end.y;
			return rec;
		}

		
	public:
		Point start;
		Point end;	
	};

	class Circle {
	public:
		Circle(Point center=Point(), uint32_t radius = 0):
			center(center) , radius(radius) {
		}

		bool touch(const Circle& cir) {
			return center.distance2(cir) <= (radius+ cir.radius)*(radius+ cir.radius); 
		}

		bool touch(const Point& point) {
			return center.distance2(point) <= radius * radius; 
		}

		bool touch(const Rectangle& rec) {
			if(rec.point_in_range(center)) {
				return true;
			}
			if( (rec.nearest(center)).distance2(center) <= radius * radius ) {
				return true;
			}
			return false;
		}
		
		Circle& move(const Point& direction) {
			center += direction;
			return *this;
		}
		
	private:
		Point center;
		uint32_t radius;
	};

	/**
	  * @brief Points
	  */
	typedef std::list<Point> Points;

	static Point walk_direct_path(Point start, Point end, uint32_t& max_len)
	{
		//KDEBUG_LOG(0,"START:(%d %d) END:(%d %d)",start.x, start.y, end.x, end.y);
		Point next_p = start;
		if(start == end) {
			max_len = 0;
			return end;
		}else { 
			Point delta_p(end.x-start.x, end.y-start.y);
			int fraction,step_x , step_y ;
			if(delta_p.x < 0 )step_x = -1; else step_x =1;
			if(delta_p.y < 0 )step_y = -1; else step_y =1;
			delta_p.x = abs(delta_p.x*2);
			delta_p.y = abs(delta_p.y*2);
			if(delta_p.x > delta_p.y) {
				fraction = delta_p.y*2 - delta_p.x;
				while(next_p.x != end.x) {
					if(fraction >=0 ) 
					{
						if( step_y) {
							if(max_len <= astar_grid_dist_oblique) {
								return next_p;
							}
							max_len -= astar_grid_dist_oblique;
						} else {
							if(max_len <= astar_grid_dist_strl) {
								return next_p;
							}
							max_len -= astar_grid_dist_strl;
						}
						next_p.y = next_p.y + step_y;
						fraction = fraction - delta_p.x;
					} else {
						if(max_len <= astar_grid_dist_strl) {
							return next_p;
						}
						max_len -= astar_grid_dist_strl;
					}
					next_p.x = next_p.x + step_x;
					fraction = fraction + delta_p.y;
				//	KDEBUG_LOG(0,"NEXT:(%d %d) FRACTION:(%d)",next_p.x, next_p.y, fraction);
				}
			} else {
				fraction = delta_p.x*2 - delta_p.y;
				while(next_p.y != end.y) {
					if(fraction >=0 ) 
					{
						if( step_x) {
							if(max_len <= astar_grid_dist_oblique) {
								return next_p;
							}
							max_len -= astar_grid_dist_oblique;
						} else {
							if(max_len <= astar_grid_dist_strl) {
								return next_p;
							}
							max_len -= astar_grid_dist_strl;
						}
						next_p.x = next_p.x + step_x;
						fraction = fraction - delta_p.y;
					} else {
						if(max_len <= astar_grid_dist_strl) {
							return next_p;
						}
						max_len -= astar_grid_dist_strl;
					}
					next_p.y = next_p.y + step_y;
					fraction = fraction + delta_p.x;
					//KDEBUG_LOG(0,"NEXT:(%d %d) FRACTION:(%d)",next_p.x, next_p.y, fraction);
				}
			}
		}
		return next_p;
	}


	static Point walk_path(Points& points, Point& direct, uint32_t& max_len)
	{
		Point end;
		if(points.empty()) {
			max_len = 0;
			return end ;
		}
		while(points.size() >=2 ) {
			Point start1 = points.back();
			//KDEBUG_LOG(0,"WALK PATH START: %d %d", start1.x, start1.y);
			points.pop_back();
			Point start2 = points.back();
			Point delta_p(start2.x - start1.x, start2.y - start1.y);
			if(delta_p.x * delta_p.y) {
				uint32_t expend = astar_grid_dist_oblique*( abs(delta_p.x));
				if(expend < max_len ) {
					max_len -= expend;
				} else {
					end = start1;
					delta_p.unitised();
					direct = delta_p;
					delta_p *= (max_len )/astar_grid_dist_oblique;
					max_len %=astar_grid_dist_oblique;
					end += delta_p;
					points.push_back(end);
//					KDEBUG_LOG(0,"WALK PATH END: %d %d", end.x, end.y);
					return end;
				}
			} else {
				uint32_t expend = astar_grid_dist_strl*( abs(delta_p.x + delta_p.y));
				if(expend < max_len ) {
					max_len -= expend;
				} else {
					end = start1;
					delta_p.unitised();
					direct = delta_p;
					delta_p *= (max_len )/astar_grid_dist_strl;
					max_len %=astar_grid_dist_strl;
					end += delta_p;
					points.push_back(end);
//					KDEBUG_LOG(0,"WALK PATH END: %d %d", end.x, end.y);
					return end;
				}
			}
			delta_p.unitised();
			direct = delta_p;
		}
		max_len = 0;
	//	KDEBUG_LOG(0,"WALK PATH END: %d %d", points.back().x, points.back().y);
		return points.back();
	}

	static	Points	line_path(Point start, Point& end, uint32_t max_len=10000000) 
	{
			Points points;
			max_len *=10;	
			uint32_t path_len = 0;
			Point next_p = start;
			Point delta_p(end.x-start.x, end.y-start.y);
			int fraction,step_x , step_y ;
			if(delta_p.x < 0 )step_x = -1; else step_x =1;
			if(delta_p.y < 0 )step_y = -1; else step_y =1;
			delta_p.x = abs(delta_p.x*2);
			delta_p.y = abs(delta_p.y*2);
			bool strl = true; 			
			if(delta_p.x > delta_p.y) {
				fraction = delta_p.y*2 - delta_p.x;
				while(next_p.x != end.x) {
					bool check = false;
					uint32_t unit_dis = astar_grid_dist_strl;
					if(fraction >=0 ) 
					{
						next_p.y = next_p.y + step_y;
						fraction = fraction - delta_p.x;
						unit_dis = astar_grid_dist_oblique;
						if(strl) check = true;
						strl = false;
					} else {
						if(!strl) check = true;
						strl = true;
					}
					next_p.x = next_p.x+ step_x;
					fraction = fraction + delta_p.y;
					path_len += unit_dis;
					if(path_len >=max_len) {
						points.push_back(next_p);
						break ;
					}
					if(check) {
						points.push_back(next_p);
					}	
				}
			} else {
				fraction = delta_p.x*2 - delta_p.y;
				while(next_p.y != end.y) {
					bool check = false;
					uint32_t unit_dis = astar_grid_dist_strl;
					if(fraction >=0 ) 
					{
						next_p.x = next_p.x + step_x;
						fraction = fraction - delta_p.y;
						unit_dis = astar_grid_dist_oblique;
						if(strl) check = true;
						strl = false;
					} else {
						if(!strl) check = true;
						strl = true;
					}
					next_p.y = next_p.y+ step_y;
					fraction = fraction + delta_p.x;
					path_len += unit_dis;
					if(path_len >=max_len) {
						points.push_back(next_p);
						break ;
					}	
					if(check) {
						points.push_back(next_p);
					}	
				}
			}
			end = next_p;
			return points;
	};

	class RandomRangePath {
	public:
		RandomRangePath(Point range_start, Point range_end):
			m_range_start(range_start) , m_range_end(range_end) {
		}

		RandomRangePath(Rectangle range):
			m_range_start(range.start) , m_range_end(range.end) {
		}
	
		RandomRangePath() {
		}

		Point get_one_path(const Point& start) {
			Point end;
			assign_random(end);
			return end;
		}
	private:
		class RightLine {
		public:
			RightLine():m_start(0,0), m_end(0,0) {
			}
			RightLine(Point start, Point end):
				m_start(start), m_end(end)
			{
			}
			Point m_start;
			Point m_end;
		};
		void assign_random(Point& result) {
			base::AverageGen<RightLine> gen;
			gen.push(RightLine(m_range_start, Point(m_range_start.x, m_range_end.y)));	
			gen.push(RightLine(m_range_start, Point(m_range_end.x, m_range_start.y)));	
			gen.push(RightLine(m_range_end, Point(m_range_start.x, m_range_end.y)));	
			gen.push(RightLine(m_range_end, Point(m_range_end.x, m_range_start.y)));	
			RightLine line = gen.get_one();
			if(line.m_start.x == line.m_end.x) {
				result.x = line.m_start.x;
				result.y =	base::ranged_random(line.m_start.y, line.m_end.y);
			} else {
				result.y = line.m_start.y;
				result.x = base::ranged_random(line.m_start.x, line.m_end.x);
			}
		}
		
	protected:
		Point m_range_start;
		Point m_range_end;
	};

public:
	TileWorld() {
	}
	/**
	  * @brief constructor
	  */
	TileWorld(const char* filename);
	/**
	 * @brief copy constructor
	 */
	TileWorld(const TileWorld& star);
	
	TileWorld& operator=(const TileWorld& star);
	/**
	  * @brief destructor
	  */
	~TileWorld();
	
	/**
	 * @brief 把对角线为[src, dst]的矩形区域设置成可行走/不可行走
	 * @param src
	 * @param dst
	 * @param walkability 1：不可行走，0：可行走
	 * @note src和dst必须构成一条斜线，坐标点必须为x,y值必须是max_row和max_column的整数倍
	 */
	bool set_grids_walkability(const Point& src, const Point& dst, int walkability);
	
	/**  
	 * @brief find a path
	 * @param src start point
	 * @param dst end point
	 * @return points that form the path in reverse, or 0 if no path
	 */
	const Points* findpath(const Point& src, const Point& dst);

	const Point search_path_end(Point start, Point end);
	/**  
	 * @brief find a linear path starts from 'src', ends when meets any roadblock, borderline or 'dst'. Support 8 directions: up, up-right, right...
	 * @param src start point
	 * @param dst end point
	 * @return points that form the path
	 */
	const Points* find_linear_path(const Point& src, const Point& dst);
	/**  
	 * @brief find points that are surrounding to the given src
	 * @param src start point
	 * @return 9 points that are surrounding to the given src
	 */
	const Points* find_surrounding_points(const Point& src);
	/**  
	 * @brief find points that are surrounding to the given src
	 * @param src start point
	 * @return 9 points that are surrounding to the given src
	 */
	const Points* find_surrounding_points2(const Point& src, uint32_t len=3);
	
	Point find_one_surrounding_points2(const Point& src, uint32_t len=3);
	
	bool check_surrounding(const Point& src, const Point& des, uint32_t len=3);
	/**  
	 * @brief find a path to escape from 'foepos' and keep a distance of 'dist'
	 * @param curpos current position of an agent
	 * @param foepos current position of a foe
	 * @param dist distance away from 'foepos'
	 * @return points that form the path in reverse, or 0 if no path
	 */
	const Points* find_escape_path(const Point& curpos, const Point& foepos, int dist);
	/**  
	 * @brief test if one point can walkable
	 * @param cur_pos current position to test
	 * @return true is walkable, or false
	 */
	bool is_pos_walkable(const Point& cur_pos);


	void get_npc_birth();

	/**
	  * @brief conversion operators, for judging whether the TileWorld obj is fine to use
	  */
	operator void*() const
		{ return reinterpret_cast<void*>(!m_err); }

private:
	/**
	  * @brief error number for TileWorld
	  */
	enum astar_error_t {
		astar_err_ok			= 0,
		astar_err_open_file		= 1,
		astar_err_read_file		= 2,
	};

	/**
	  * @brief Index to a Grid
	  */
	class GridIndex {
	public:
		/**
		  * @brief Default Constructor
		  */
		GridIndex(uint32_t r = 0, uint32_t c = 0)
			{ row = r; column = c; }

	public:
		friend bool operator ==(const GridIndex& i1, const GridIndex& i2)
			{ return (i1.row == i2.row) && (i1.column == i2.column); }

		friend bool operator !=(const GridIndex& i1, const GridIndex& i2)
			{ return !(i1 == i2); }

		friend GridIndex operator -(const GridIndex& lhs, const GridIndex& rhs)
			{ GridIndex ret(lhs); ret.row -= rhs.row; ret.column -= rhs.column; return ret; }

	public:
		/*! row */
		uint32_t	row;
		/*! column */
		uint32_t	column;
	};

	/**
	 * @brief A* algorithm for path finding
	 */
	class Grid {
	public:
		Grid()
			{ parent = 0; child = 0; f = 0; g = 0; }

	public:
		friend bool operator ==(const Grid& g1, const Grid& g2)
			{ return (g1.idx == g2.idx); }

	public:
		/*! used during the search to record the parent of successor nodes */
		Grid*	parent;
		/*!
		  *  1. used to link all closed node together
		  *  2. (not used now) used after the search for the application to view the search in reverse (not used now)
		  */
		Grid*	child;

		/*! sum of cumulative cost of predecessors and self and heuristic */
		uint32_t	f;
		/*! cost of this node + it's predecessors */
		uint32_t	g;

		/*! index to this grid */
		GridIndex	idx;
		/*! Real Data in a Grid */
		GridData	grid_data;

		uint32_t grid_value;
	};

private:
	void alloc_map();

	void try_add_successor(const GridIndex& idx, Grid* parent, uint32_t cost = astar_grid_dist_strl);
	void form_path(Grid* goal, const Point& dst);

	GridPointer get_grid(const GridIndex& idx);

	void init_closed_list()
		{ m_closed_list.parent = &m_closed_list; m_closed_list.child = 0; }
	void add_to_closed_list(Grid* g)
		{ m_closed_list.parent->child = g; m_closed_list.parent = g; g->child = 0; }

	uint32_t distance(const GridIndex& start, const GridIndex& end) const
		{ return (abs((int)(end.row - start.row)) + abs((int)(end.column - start.column))) * astar_grid_dist_strl; }

	GridIndex point_to_grid_idx(const Point& pt) const
		{ return GridIndex (pt.y / m_ppg_y, pt.x / m_ppg_x); }
	Point grid_idx_to_point(const GridIndex& idx) const
		{ return Point( idx.column * m_ppg_x, idx.row * m_ppg_y); }

	bool out_of_range(const GridIndex& idx) const
		{ return (idx.row >= m_max_grids_row) || (idx.column >= m_max_grids_column); }

private:
	/**
	  * @brief For sorting the heap the STL needs compare function that lets us compare the f value of two nodes
	  */
	static bool grid_cmp(const Grid* g1, const Grid* g2)
		{ return (g1->f > g2->f); }

private:
	/*! max grids of a row */
	uint32_t	m_max_grids_row;
	/*! max grids of a column */
	uint32_t	m_max_grids_column;
	/*! pixels per grid for coordinate x */
	uint32_t	m_ppg_x;
	/*! pixels per grid for coordinate y */
	uint32_t	m_ppg_y;
	/*! points that form a path */
	Points		m_points;

	/*! map load from file */
	GridPointer*	m_map;

	/*! pointer to the current position that is being searched */
	Grid*						m_cur_grid;
	/*! pointer to the target position */
	Grid*						m_goal_grid;
	/*! open list, simple vector but used as a heap */
	std::vector<GridPointer>	m_open_list;
	/*! closed list, simple Grid but used as a head to link all closed node together */
	Grid						m_closed_list;

	/*! error number */
	int	m_err;

	Points m_birth_points;


};

template <typename GridData>
inline
TileWorld<GridData>::TileWorld(const char* filename)
{
	CCLOG("TileWorld init %s", filename);
	m_err = astar_err_ok;
	m_map = 0;

	init_closed_list();

	m_open_list.reserve(50);
	//m_points.reserve(50);

	std::string fullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(filename);

	unsigned long	_size;
	char			*file = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullFilePath.c_str() , "rb", &_size);

	CCAssert(file, "tileworld .map read failed");

	//CCFileData data(fullFilePath.c_str(), "rb");
	//unsigned char* file = new unsigned char[data.getSize()];
	//memcpy(file, data.getBuffer(), data.getSize());

	//unsigned long	_size;
	//unsigned char	*file = (unsigned char*)CCFileUtils::sharedFileUtils()->getFileData(fullFilePath.c_str() , "rb", &_size);

	// open file
	//std::ifstream fin(filename);
	//assert(fin.is_open());
	//if (!fin) {
	//	m_err = astar_err_open_file;
	//	return;
	//}

	// read head info from map data file
	//fin >> m_max_grids_column >> m_max_grids_row;
	m_max_grids_row = 0; m_max_grids_column = 0;
	uint32_t  cell_edge = 0;

	memcpy((unsigned char*)&cell_edge, file, sizeof(cell_edge));
	file += sizeof(cell_edge);
	memcpy((unsigned char*)&m_max_grids_row, file, sizeof(m_max_grids_row));
	file += sizeof(m_max_grids_row);
	memcpy((unsigned char*)&m_max_grids_column, file, sizeof(m_max_grids_column));
	file += sizeof(m_max_grids_column);
	//fin.read((char*)&cell_edge, sizeof(cell_edge));
	//fin.read((char*)&m_max_grids_row, sizeof(m_max_grids_row));
	//fin.read((char*)&m_max_grids_column, sizeof(m_max_grids_column));
	m_ppg_x = m_ppg_y = 1;
	//if (!fin) {
	//	m_err = astar_err_read_file;
	//	assert(false);
	//	return;
	//}

	// alloc memory to hold the given map
	alloc_map();

	// load map data from the given file
	for (uint32_t j = 0; j != m_max_grids_column; ++j) {
		for (uint32_t i = 0; i != m_max_grids_row; ++i) {
			//fin.read((char*)&m_map[i][j].grid_data.m_data, sizeof((m_map[i][j].grid_data.m_data)));
			memcpy((unsigned char*)&m_map[i][j].grid_data.m_data, file, sizeof(m_map[i][j].grid_data.m_data));
			file += sizeof(m_map[i][j].grid_data.m_data);
			//fin >> m_map[i][j].grid_data;
			m_map[i][j].idx.row    = i;
			m_map[i][j].idx.column = j;
		}
	}
	
	//for (uint32_t j = 0; j != m_max_grids_column; ++j) {
	//	for (uint32_t i = 0; i != m_max_grids_row; ++i) {
	//		//fin.read((char*)&m_map[i][j].grid_value, sizeof((m_map[i][j].grid_value)));
	//		memcpy((unsigned char*)&m_map[i][j].grid_value, file, sizeof(int));
	//		file += sizeof(int);
	//		//fin >> m_map[i][j].grid_value;
	//	//	KINFO_LOG(0,"LOAD MAP %d %d:%d", i, j,m_map[i][j].grid_value);
	//	}
	//}

	//if (!fin) {
	//	m_err = astar_err_read_file;
	//	return;
	//}
}


template <typename GridData>
inline 
TileWorld<GridData>::TileWorld(const TileWorld<GridData>& star)
{
	m_err              = star.m_err;
	m_max_grids_column = star.m_max_grids_column;
	m_max_grids_row    = star.m_max_grids_row;
	m_ppg_x            = star.m_ppg_x;
	m_ppg_y            = star.m_ppg_y;
	m_points           = star.m_points;
	m_cur_grid         = star.m_cur_grid;
	m_goal_grid        = star.m_goal_grid;
	m_open_list        = star.m_open_list;
	m_map              = 0;
	init_closed_list();

	// copy map
	alloc_map();
	for (uint32_t i = 0; i != m_max_grids_row; ++i){
		for (uint32_t j = 0; j != m_max_grids_column; ++j){
			m_map[i][j].grid_data = star.m_map[i][j].grid_data;
			m_map[i][j].idx       = star.m_map[i][j].idx;
		}
	}
}
	
template <typename GridData>
inline 
TileWorld<GridData>& TileWorld<GridData>::operator=(const TileWorld<GridData>& star)
{
	m_err              = star.m_err;
	m_max_grids_column = star.m_max_grids_column;
	m_max_grids_row    = star.m_max_grids_row;
	m_ppg_x            = star.m_ppg_x;
	m_ppg_y            = star.m_ppg_y;
	m_points           = star.m_points;
	m_cur_grid         = star.m_cur_grid;
	m_goal_grid        = star.m_goal_grid;
	m_open_list        = star.m_open_list;
	m_map              = 0;
	init_closed_list();

	// copy map
	alloc_map();
	for (uint32_t i = 0; i != m_max_grids_row; ++i){
		for (uint32_t j = 0; j != m_max_grids_column; ++j){
			m_map[i][j].grid_data = star.m_map[i][j].grid_data;
			m_map[i][j].idx       = star.m_map[i][j].idx;
		}
	}
	return *this;
}

template <typename GridData>
inline
TileWorld<GridData>::~TileWorld()
{
	if (m_map) {
		delete [] m_map[0];
		delete [] m_map;
	}
}

	

template <typename GridData>
inline 
void TileWorld<GridData>::get_npc_birth() {
	m_birth_points.clear();
	std::map<uint32_t, Point> points_map;
	for (uint32_t i = 0; i != m_max_grids_row; ++i) {
		for (uint32_t j = 0; j != m_max_grids_column; ++j) {
			if (m_map[i][j].grid_data.is_npc_birth())
			{
				points_map[m_map[i][j].grid_value] = grid_idx_to_point(GridIndex(i,j));
			}	
		}
	}
	typename std::map<uint32_t, Point>::iterator itr = points_map.begin();
	for( ;itr!= points_map.end(); ++itr) {
		m_birth_points.push_back(itr->second);
	}
}

template <typename GridData>
bool TileWorld<GridData>::set_grids_walkability(const Point& src, const Point& dst, int walkability)
{
	if(src.x == dst.x || src.y == dst.y){
		return false;
	}
	if(src.x % m_ppg_x != 0 || src.y % m_ppg_y != 0){
		return false;
	}
	if(dst.x % m_ppg_x != 0 || dst.y % m_ppg_y != 0){
		return false;
	}

	GridIndex start_idx = point_to_grid_idx(src);
	GridIndex end_idx   = point_to_grid_idx(dst);

	uint32_t min_x_index = start_idx.column > end_idx.column ? end_idx.column : start_idx.column;
    uint32_t max_x_index = start_idx.column < end_idx.column ? end_idx.column : start_idx.column;
	uint32_t min_y_index = start_idx.row > end_idx.row ? end_idx.row: start_idx.row;
	uint32_t max_y_index = start_idx.row < end_idx.row ? end_idx.row: start_idx.row;
	
	for (uint32_t i = min_y_index; i < max_y_index; ++i) {
		if (i >= m_max_grids_row) {
			break;
		}

		for(uint32_t j = min_x_index; j < max_x_index; ++j) {
			if(j >= m_max_grids_column) {
				break;
			}
			m_map[i][j].grid_data.set_walkability(walkability);
		}
	}

	return true;
}

template <typename GridData>
inline const typename TileWorld<GridData>::Points* 
TileWorld<GridData>::findpath(const Point& src, const Point& dst)
{
	m_points.clear();
	m_cur_grid = 0;

	GridIndex start_idx = point_to_grid_idx(src);
	GridIndex end_idx   = point_to_grid_idx(dst);

	if (start_idx == end_idx) {
		m_points.push_back(dst);		
		return &m_points;
	}

	Grid* start_grid = get_grid(start_idx);
	m_goal_grid      = get_grid(end_idx);
	if (!start_grid || !m_goal_grid || !m_goal_grid->grid_data.is_walkable()) {
		return 0;
	}

	m_goal_grid->parent = 0;

	start_grid->parent = 0;
	start_grid->g      = 0;
	start_grid->f      = distance(start_idx, end_idx);
	// heap now unsorted
	m_open_list.push_back(start_grid);
	// sort back element into heap
	std::push_heap(m_open_list.begin(), m_open_list.end(), grid_cmp);

	do {
		// Pop the best node (the one with the lowest f) 
		m_cur_grid = m_open_list.front(); // get pointer to the node
		std::pop_heap(m_open_list.begin(), m_open_list.end(), grid_cmp);
		m_open_list.pop_back();

		// add current node to the closed list
		add_to_closed_list(m_cur_grid);

		// path found
		if (m_cur_grid == m_goal_grid) {
			break;
		}

		// try each adjacent node
		const GridIndex& idx = m_cur_grid->idx;
		try_add_successor(GridIndex(idx.row, idx.column - 1), m_cur_grid);
		try_add_successor(GridIndex(idx.row, idx.column + 1), m_cur_grid);
		try_add_successor(GridIndex(idx.row - 1, idx.column), m_cur_grid);
		try_add_successor(GridIndex(idx.row + 1, idx.column), m_cur_grid);
		try_add_successor(GridIndex(idx.row + 1, idx.column - 1), m_cur_grid, astar_grid_dist_oblique);
		try_add_successor(GridIndex(idx.row - 1, idx.column - 1), m_cur_grid, astar_grid_dist_oblique);
		try_add_successor(GridIndex(idx.row + 1, idx.column + 1), m_cur_grid, astar_grid_dist_oblique);
		try_add_successor(GridIndex(idx.row - 1, idx.column + 1), m_cur_grid, astar_grid_dist_oblique);
	} while (m_open_list.size());

	// clear f
	Grid* node = m_closed_list.child;
	while (node) {
		node->f = 0;
		node    = node->child;
	}
	for (typename std::vector<GridPointer>::iterator it = m_open_list.begin(); it != m_open_list.end(); ++it) {
		(*it)->f = 0;
	}

	m_open_list.clear();
	init_closed_list();

	// path found
	if (m_goal_grid->parent) {
		form_path(m_goal_grid, dst);
		return &m_points;
	}

	return 0;
}

template <typename GridData>
inline const typename TileWorld<GridData>::Point 
TileWorld<GridData>::search_path_end(Point start, Point end)
{
		Point next_p = start;
		if(start == end) {
			return end;
		}else { 
			Point delta_p(end.x-start.x, end.y-start.y);
			int fraction,step_x , step_y ;
			if(delta_p.x < 0 )step_x = -1; else step_x =1;
			if(delta_p.y < 0 )step_y = -1; else step_y =1;
			delta_p.x = abs(delta_p.x*2);
			delta_p.y = abs(delta_p.y*2);
			if(delta_p.x > delta_p.y) {
				fraction = delta_p.y*2 - delta_p.x;
				while(next_p.x != end.x) {
					Point now_p = next_p;
					if(fraction >=0 ) 
					{
						now_p.y = next_p.y + step_y;
						fraction = fraction - delta_p.x;
					}
					now_p.x = next_p.x + step_x;
					fraction = fraction + delta_p.y;
					GridIndex idx = point_to_grid_idx(now_p);
					Grid* grid = get_grid(idx);
					if(!grid || !grid->grid_data.is_walkable()) {
						return next_p;
					} else {
						next_p = now_p;
					}
				}
			} else {
				fraction = delta_p.x*2 - delta_p.y;
				while(next_p.y != end.y) {
					Point now_p = next_p;
					if(fraction >=0 ) 
					{
						now_p.x = next_p.x + step_x;
						fraction = fraction - delta_p.y;
					}
 					now_p.y = next_p.y + step_y;
					fraction = fraction + delta_p.x;
					GridIndex idx = point_to_grid_idx(now_p);
					Grid* grid = get_grid(idx);
					if(!grid || !grid->grid_data.is_walkable()) {
						return next_p;
					} else {
						next_p = now_p;
					}				
				}
			}
		}
		return next_p;
}


template <typename GridData>
inline const typename TileWorld<GridData>::Points* 
TileWorld<GridData>::find_linear_path(const Point& src, const Point& dst)
{
	m_points.clear();

	GridIndex start_idx = point_to_grid_idx(src);
	GridIndex end_idx   = point_to_grid_idx(dst);

	if (start_idx == end_idx) {
		m_points.push_back(dst);
		return &m_points;
	}

	Grid* start_grid = get_grid(start_idx);
	int row_diff = end_idx.row - start_idx.row;
	int col_diff = end_idx.column - start_idx.column;
	if ((start_grid == 0) || !start_grid->grid_data.is_walkable()
			|| ((row_diff != 0) && (col_diff != 0) && (abs(row_diff) != abs(col_diff)))) {
		m_points.push_back(src);
		return &m_points;
	}

	if (row_diff > 0) {
		row_diff = 1;
	} else if (row_diff < 0) {
		row_diff = -1;
	}

	if (col_diff > 0) {
		col_diff = 1;
	} else if (col_diff < 0) {
		col_diff = -1;
	}

	GridIndex prev_idx = start_idx;
	GridIndex cur_idx(prev_idx.row + row_diff, prev_idx.column + col_diff);
	for ( ; ; ) {
		Grid* g = get_grid(cur_idx);
		if (g && g->grid_data.is_walkable()) {
			if (cur_idx != end_idx) {
				prev_idx        = cur_idx;
				cur_idx.row    += row_diff;
				cur_idx.column += col_diff;
			} else {
				m_points.push_back(dst);
				break;
			}
		} else {
			if (prev_idx != start_idx) {
				Point pt = grid_idx_to_point(prev_idx);
				if (row_diff == 0) {
					pt.y  = src.y;
					pt.x += m_ppg_x / 2;
				} else if (col_diff == 0) {
					pt.x  = src.x;
					pt.y += m_ppg_y / 2;
				} else {
					pt.x += m_ppg_x / 2;
					pt.y += m_ppg_y / 2;
				}
				m_points.push_back(pt);
			} else {
				m_points.push_back(src);
			}
			break;
		}
	}

	return &m_points;
}

template <typename GridData>
inline const typename TileWorld<GridData>::Points* 
TileWorld<GridData>::find_surrounding_points(const Point& src)
{
	m_points.clear();

	GridIndex idx = point_to_grid_idx(src);

	Grid* grid = get_grid(idx);
	if (grid == 0) {
		m_points.push_back(src);
		return &m_points;
	}

	Point pt = grid_idx_to_point(idx);
	for (int i = 0; i != 3; ++i) {
		for (int j = 0; j != 3; ++j) {
			m_points.push_back(Point(pt.x + m_ppg_x / 2 * i, pt.y + m_ppg_y / 2 * j));
		}
	}

	return &m_points;
}

template <typename GridData>
inline const typename TileWorld<GridData>::Points* 
TileWorld<GridData>::find_surrounding_points2(const Point& src, uint32_t ulen)
{
	m_points.clear();

	GridIndex idx = point_to_grid_idx(src);
	Grid* grid = get_grid(idx);
	if (grid == 0) {
		m_points.push_back(src);
		return &m_points;
	}

	GridIndex dst_idx;
	//
	int dst_row = 0, dst_col = 0;

	int len = ulen;
	for(int i=1; i < len ; ++i) {
		dst_row = idx.row + i;
		dst_col = idx.column;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}

	for(int i=-1; i > -len ; --i) {
		dst_row = idx.row + i;
		dst_col = idx.column;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}

	for(int i=1; i < len ; ++i) {
		dst_row = idx.row;
		dst_col = idx.column+i;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}

	for(int i=-1; i > -len ; --i) {
		dst_row = idx.row;
		dst_col = idx.column +i;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}

	for(int i=1; i < len ; ++i) {
		dst_row = idx.row + i;
		dst_col = idx.column +i;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}

	for(int i=-1; i > -len ; --i) {
		dst_row = idx.row +i;
		dst_col = idx.column +i;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}

	for(int i=1; i < len ; ++i) {
		dst_row = idx.row + i;
		dst_col = idx.column -i;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}

	for(int i=-1; i > -len ; --i) {
		dst_row = idx.row +i;
		dst_col = idx.column -i;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}
	
	for(int i=1; i < len ; ++i) {
		dst_row = idx.row + i;
		dst_col = idx.column +i;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}

	for(int i=-1; i > -len ; --i) {
		dst_row = idx.row +i;
		dst_col = idx.column +i;	
		if (dst_row < 0 || dst_col < 0 ) {
			break;
		}
		dst_idx = GridIndex(dst_row, dst_col);
		Grid* grid = get_grid(dst_idx);
		if ( !grid || !grid->grid_data.is_walkable()) {
			break;
		}
		m_points.push_back(grid_idx_to_point(dst_idx));
	}

/*
	std::set<Point> test_points;
	std::set<Point> finish_points;
	test_points.insert(src);
	while(!test_points.empty()) {
		typename std::set<Point>::iterator itr = test_points.begin();
		finish_points.insert(*itr);
		m_points.push_back(*itr);
		for (int i = -1; i !=2; ++i) {
			for (int j = -1; j != 2; ++j) {
				GridIndex idx = point_to_grid_idx(*itr);
				dst_row = idx.row + i ;
				dst_col = idx.column + j ;
				if (dst_row < 0 || dst_col < 0 || abs(dst_row - src.y) > (int)len || abs(dst_col - src.x)> (int)len ) {
					continue;
				}
				dst_idx = GridIndex(dst_row, dst_col);
				Grid* grid = get_grid(dst_idx);
				if ( !(i || j) || !grid || grid->f || !grid->grid_data.is_walkable()) {
					continue;
				}
				Point now_point = grid_idx_to_point(dst_idx);
				if(!finish_points.count(now_point))
					test_points.insert(now_point);	
			}
		}
		test_points.erase(itr);
	} */
/*
	for (int i = -length; i != length+1; ++i) {
		for (int j = -length; j != length+1; ++j) {
			dst_row = idx.row + i * adj;
			dst_col = idx.column + j * adj;
			if (dst_row < 0 || dst_col < 0) {
				continue;
			}
			
			dst_idx = GridIndex(dst_row, dst_col);
			Grid* grid = get_grid(dst_idx);
			if (!grid || grid->f || !grid->grid_data.is_walkable()) {
				continue;
			}
			
			m_points.push_back(grid_idx_to_point(dst_idx));
		}
	}
*/	
	if (m_points.size() == 0) {
		m_points.push_back(src);
	}

	return &m_points;
}
template <typename GridData>
inline typename TileWorld<GridData>::Point
TileWorld<GridData>::find_one_surrounding_points2(const Point& src, uint32_t len) {
	const Points* points = find_surrounding_points2(src, len);
	base::AverageGen<Point> gen;
	TileWorld<>::Points::const_iterator itr = points->begin();
	for(; itr!=points->end(); ++itr) {
		gen.push(*itr);
	}
	return gen.get_one();
}
	
template <typename GridData>
inline bool TileWorld<GridData>::check_surrounding(const Point& src, const Point& des, uint32_t len) {
	return abs(des.x-src.x) <= (int)len && abs(des.y - src.y) <= (int)len;
}

template <typename GridData>
inline const typename TileWorld<GridData>::Points* 
TileWorld<GridData>::find_escape_path(const Point& curpos, const Point& foepos, int dist)
{
	static int sign[] = { -1, 1 };

	int grid_num = dist / m_ppg_x;
	Grid* grid;
	GridIndex foe_idx = point_to_grid_idx(foepos);
	GridIndex dst_idx;
	int adj = 0;

	if (foe_idx.column + grid_num < m_max_grids_column - 1) {
		if (foe_idx.row + 3 <= m_max_grids_row && foe_idx.row >= 3) {
			adj = (rand() % 4) * (rand() % 2);
		}
		if (adj) {
			dst_idx = GridIndex(foe_idx.row + adj, foe_idx.column + grid_num);
		} else {
			dst_idx = GridIndex(foe_idx.row, foe_idx.column + grid_num + 1);
		}
	}
	grid = get_grid(dst_idx);
	if (grid && grid->grid_data.is_walkable()) {
		goto ret;
	}

	if (foe_idx.column > grid_num) {
		if (foe_idx.row + 3 <= m_max_grids_row && foe_idx.row >= 3) {
			adj = (rand() % 4) * (rand() % 2);
		}
		if (adj) {
			dst_idx = GridIndex(foe_idx.row + adj, foe_idx.column - grid_num);
		} else {
			dst_idx = GridIndex(foe_idx.row, foe_idx.column - grid_num - 1);
		}
	}
	grid = get_grid(dst_idx);
	if (grid && grid->grid_data.is_walkable()) {
		goto ret;
	}

	if (foe_idx.row + grid_num < m_max_grids_row - 1) {
		if (foe_idx.column + 3 <= m_max_grids_column && foe_idx.column >= 3) {
			adj = (rand() % 4) * (rand() % 2);
		}
		if (adj) {
			dst_idx = GridIndex(foe_idx.row + grid_num, foe_idx.column + adj);
		} else {
			dst_idx = GridIndex(foe_idx.row + grid_num + 1, foe_idx.column);
		}
	}
	grid = get_grid(dst_idx);
	if (grid && grid->grid_data.is_walkable()) {
		goto ret;
	}

	if (foe_idx.column + 3 <= m_max_grids_column && foe_idx.column >= 3) {
		adj = (rand() % 4) * (rand() % 2);
	}
	if (adj) {
		dst_idx = GridIndex(foe_idx.row - grid_num, foe_idx.column + adj);
	} else {
		dst_idx = GridIndex(foe_idx.row - grid_num - 1, foe_idx.column);
	}

ret:
	const Points* pts = findpath(curpos, grid_idx_to_point(dst_idx));
	if (pts) {
		return pts;
	}

	// path found
	if (m_cur_grid && m_cur_grid->parent) {
		form_path(m_cur_grid, grid_idx_to_point(m_cur_grid->idx));
		return &m_points;
	}

	return 0;
}

template <typename GridData>
inline bool
TileWorld<GridData>::is_pos_walkable(const Point& cur_pos)
{
	GridIndex pos_idx = point_to_grid_idx(cur_pos);

	Grid* pos_grid = get_grid(pos_idx);
	if (!pos_grid || !pos_grid->grid_data.is_walkable()) {
		return false;
	}
	
	return true;
}

//------------------------------------------------------------------------
// Private Methods
//
template <typename GridData>
inline void
TileWorld<GridData>::alloc_map()
{
	//alloc memory 
	m_map	 = new GridPointer[m_max_grids_row];
	m_map[0] = new Grid[m_max_grids_row * m_max_grids_column];
	for (uint32_t i = 1; i != m_max_grids_row; ++i){
		m_map[i] = m_map[0] + i * m_max_grids_column;
	}
}

template <typename GridData>
inline void
TileWorld<GridData>::try_add_successor(const GridIndex& idx, Grid* parent, uint32_t cost)
{
	Grid* grid = get_grid(idx);
/*todo: add bulk for more check*/
	if (!grid || grid->f || !grid->grid_data.is_walkable()) {
		return;
	}

	grid->g      = parent->g + cost;
	grid->f      = grid->g + distance(idx, m_goal_grid->idx);
	grid->parent = parent;

	// heap now unsorted
	m_open_list.push_back(grid);
	// sort back element into heap
	std::push_heap(m_open_list.begin(), m_open_list.end(), grid_cmp);
}

template <typename GridData>
inline void
TileWorld<GridData>::form_path(Grid* goal, const Point& dst)
{
	Grid* cur	= goal;
	Grid* prev1 = cur->parent;
	Grid* prev2 = prev1->parent;
/*	Grid* prev_inused_grid = cur;
	prev_inused_grid->parent = 0;
	while (prev2) {
		if (((cur->idx - prev1->idx) != (prev1->idx - prev2->idx)) && (cur != prev_inused_grid)) {
			prev_inused_grid->parent = prev1;
			prev_inused_grid = prev1;
		}
		cur   = prev1;
		prev1 = prev2;
		prev2 = prev1->parent;
	}
	prev_inused_grid->parent = prev1;

	cur   = goal;
	prev1 = cur->parent;
	prev2 = prev1->parent;*/
	m_points.push_back(dst);
	while (prev2) {
		if ((cur->idx - prev1->idx) != (prev1->idx - prev2->idx)) {
			m_points.push_back(grid_idx_to_point(prev1->idx));
		}
		cur   = prev1;
		prev1 = prev2;
		prev2 = prev1->parent;
	}
}

template <typename GridData>
inline typename TileWorld<GridData>::GridPointer
TileWorld<GridData>::get_grid(const GridIndex& idx)
{
	if (!out_of_range(idx)) {
		return &(m_map[idx.row][idx.column]);
	}

	return 0;
}

inline bool operator<(const TileWorld<>::Point& a, const TileWorld<>::Point& b) {
	return a.x < b.x || ((a.x == b.x) && (a.y < b.y));
}

} // end of namespace taomee

#endif // LIBTAOMEEPP_ASTAR_HPP_
