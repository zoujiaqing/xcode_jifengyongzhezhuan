//
//  MathUtil.h
//  SeerDemo
//
//  Created by wuwei on 11/21/11.
//  Copyright 2011 Taomee.Inc. All rights reserved.
//
// This file defines some common geometry-releated data structures and algorithms used in Seer mobile edition.
// 这个文件中定义了一些几何相关的数据结构和算法，用于赛尔号移动版

#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

#include "cocos2d.h"

// line segment
// 线段
struct Segment
{
    cocos2d::CCPoint start;
    cocos2d::CCPoint end;
    
    Segment(){};
    Segment(cocos2d::CCPoint s, cocos2d::CCPoint e)
    :start(s),end(e){}
};

// polygon
// 多边形
typedef std::vector<cocos2d::CCPoint> PointArray;
struct Polygon
{
    PointArray points;
    Polygon(){};
    Polygon(const cocos2d::CCRect& rect)
    {
        points.push_back(rect.origin);
        points.push_back(ccp(rect.origin.x, rect.origin.y + rect.size.height));
        points.push_back(ccp(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height));
        points.push_back(ccp(rect.origin.x + rect.size.width, rect.origin.y));
    }
    cocos2d::CCPoint getPoint(int index) const  { return points[index]; }
    void addPoint(cocos2d::CCPoint p) { points.push_back(p); }
    void clear() { points.clear(); }
    size_t size() { return points.size(); }
};

// circle
// 圆
struct Circle
{
    cocos2d::CCPoint center;
    float radius;
    
    Circle(cocos2d::CCPoint c, float r)
    :center(c),radius(r){}
};

// calculate the cross product of two vectors
// 计算向量叉积
inline cocos2d::CCPoint vectorCross(const cocos2d::CCPoint& v1, const cocos2d::CCPoint& v2)
{
    return ccp(v1.y - v2.y, v2.x - v1.x);
}

// calculate the shortest distance from a point to a line segment
// 计算点到线段的最短距离
float minDist(const cocos2d::CCPoint& p, const Segment& s);

// calculate the intersection of two segments, assuming they intersect
// 计算两条线段交点，假设他们相交
cocos2d::CCPoint intersection(const Segment& s1, const Segment& s2);

// determine if two line segments intersect with each other
// 判定两条线段是否相交
bool isTwoSegmentsIntersect(const Segment& s1, const Segment& s2);
bool isTwoSegmentsIntersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

// determine if a line segment intersects with a polygon
// 判定一条线段和一个多边形是否相交
bool isSegmentIntersectWithPolygon(const Segment& s, const struct Polygon& p);

// determin if a line segment intersects with a rectangle
// 判定一条线段和一个矩形是否相交
bool isSegmentIntersectWithRect(const Segment& s, const cocos2d::CCRect& rect);

// determine if a line segment intersects with a circle
// 判定一条线段和一个圆是否相交
bool isSegmentIntersectWithCircle(const Segment& s, const Circle& c);

// determine if a rect overlaps a circle
// 判定一个矩形是否和一个圆有重叠
bool isRectOverlapCircle(const cocos2d::CCRect& rect, const Circle& c);

// determine if a rect overlaps a polygon
// 判定一个矩形是否和一个多边形有重叠
bool isRectOverlapPolygon(const cocos2d::CCRect& rect, const struct Polygon& p);

// determine if a point is inside a rectangle, including on the border
// 判定一个点是否在矩形内（包括在边界上）
bool isPointInRect(const cocos2d::CCPoint& p, const cocos2d::CCRect& rect);

// determine if a point is inside a circle, including on the border
// 判定一个点是否在圆内（包括在边界上）
bool isPointInCircle(const cocos2d::CCPoint& p, const Circle& c);

// determine if a point is inside a polygon, including on the border
// 判定一个点是否在多边形内（包括在边界上）
bool isPointInPolygon(const cocos2d::CCPoint& point, const struct Polygon& polygon);

// generate a polygon to simulate a fan
// 生成一个多边形来近似一个扇形
// 参数：1.c 扇形圆心
//      2.r 扇形半径
//      2.angle  扇形夹角
//      3.rotation 扇形中心轴相对x轴正向的旋转角
//      4.outPolygon 存储多边形数据的引用
void genFanPolygon(const cocos2d::CCPoint& c, float r, float angle, float rotation, struct Polygon& outPolygon);

// rotate a rect
// 旋转矩形
void rotateRect(const cocos2d::CCRect& rect, float angle, const cocos2d::CCPoint& pivot, struct Polygon& outPolygon);


inline int ranged_random(int a, int b)
{
	int max = a>b?a:b;
	int min = a<=b?a:b;
	// generates ranged random number
	return (rand() % (max - min + 1)) + min;
}

#endif