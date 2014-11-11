//
//  MathUtil.cpp
//  SeerDemo
//
//  Created by wuwei on 11/21/11.
//  Copyright 2011 Taomee.Inc. All rights reserved.
//
#include "MathUtil.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#define fmax max
#define fmin min
#endif
using namespace cocos2d;

// calculate the shortest distance from a point to a line segment
// 计算点到线段的最短距离
float minDist(const cocos2d::CCPoint& p, const Segment& s)
{
    CCPoint a = s.start;
    CCPoint b = s.end;
    
    // calculate the dot product of ab & ap
    // 计算向量ab和ap的点积
    CCPoint ab = ccp(b.x - a.x, b.y - a.y);
    CCPoint ap = ccp(p.x - a.x, p.y - a.y);
    float f = ccpDot(ab, ap);
    
    // if f <= 0, angle b-a-p is larger than or equal to 90, |ap| is the shortest distance
    // 若f <= 0,则角b-a-p大于等于90度，|ap|为最短距离
    if(f <= 0) return ccpDistance(a, p);
    
    // if..., |bp| is the shortest distance
    // 若...，|bp|为最短距离
    float d = ccpDot(ab, ab);
    if(f >= d) return ccpDistance(b, p);
    
    // the projection point of p on s, |pq| is the shortest distance
    // 点p在线段s上的投影点，|pq|为最短距离
    f = f/d;
    CCPoint q = ccp(a.x + f * ab.x, a.y + f * ab.y);
    return ccpDistance(p, q);
}

// calculate the intersection of two segments, assuming they intersect
// 计算两条线段交点，假设他们相交
CCPoint intersection(const Segment& s1, const Segment& s2)
{
    float a1 = s1.end.y - s1.start.y;
    float b1 = s1.start.x - s1.end.x;
    float c1 = -b1 * s1.start.y - a1 * s1.start.x;
    
    float a2 = s2.end.y - s2.start.y;
    float b2 = s2.start.x - s2.end.x;
    float c2 = -b2 * s2.start.y - a2 * s2.start.x;
    
    float d = b1 * a2 - b2 * a1;
    
    // on this occasion, the two segments overlap
    // we don't calculate accurately, just return an approximate point
    // 这种情况下，两线段重合，不做精确计算，只取一个近似点
    if(d == 0)
    {
        return s2.start;
    }
    
    float x = (c1 * b2 - c2 * b1) / d;
    float y = (c2 * a1 - c1 * a2) / d;
    
    return ccp(x,y);
}

// determine if two line segments intersect with each other
// 判定两条线段是否相交
bool isTwoSegmentsIntersect(const Segment& s1, const Segment& s2)
{
    return isTwoSegmentsIntersect(s1.start.x, s1.start.y, s1.end.x, s1.end.y, s2.start.x, s2.start.y, s2.end.x, s2.end.y);
}

bool isTwoSegmentsIntersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    if(fmax(x1, x2) < fmin(x3, x4) ||
       fmax(x3, x4) < fmin(x1, x2) ||
       fmax(y1, y2) < fmin(y3, y4) ||
       fmax(y3, y4) < fmin(y1, y2) )
	{
		return false;
	}
	
	float m1 = (x2-x1) * (y3-y1) - (x3-x1) * (y2-y1);
	float m2 = (x2-x1) * (y4-y1) - (x4-x1) * (y2-y1);
	
	//if(m1 * m2 >= 0)
    if(m1 * m2 > 0)
		return false;
	
	float m3 = (x4-x3) * (y1-y3) - (x1-x3) * (y4-y3);
	float m4 = (x4-x3) * (y2-y3) - (x2-x3) * (y4-y3);
	
	//if(m3 * m4 >= 0)
    if(m3 * m4 > 0)
		return false;
	
	return true;
}

// determine if a line segment intersects with a polygon
// 判定一条线段和一个多边形是否相交
bool isSegmentIntersectWithPolygon(const Segment& s, const struct Polygon& p)
{
    Segment ps;
    
    int numSides = p.points.size();
    for(int i = 0; i < numSides; ++i)
    {
        if(i < numSides - 1)
        {
            ps.start = p.getPoint(i);
            ps.end   = p.getPoint(i+1);
        }
        else
        {
            ps.start = p.getPoint(i);
            ps.end   = p.getPoint(0);
        }
        
        if(isTwoSegmentsIntersect(s, ps))
        {
            return true;
        }
    }
    
    return false;
}

// determin if a line segment intersects with a rectangle
// 判定一条线段和一个矩形是否相交
bool isSegmentIntersectWithRect(const Segment& s, const cocos2d::CCRect& rect)
{
    struct Polygon polygon(rect);
    return isSegmentIntersectWithPolygon(s, polygon);
}

// determine if a line segment intersects with a circle, including tangency
// 判定一条线段和一个圆是否相交, 包括相切
bool isSegmentIntersectWithCircle(const Segment& s, const Circle& c)
{
    // calculate the shortest distance from the circle center to s
    // 计算圆心到线段的最短距离
    float dist = minDist(c.center, s);
    return dist <= c.radius;
}

// determine if a rect overlaps a circle
// 判定一个矩形是否和一个圆重叠
bool isRectOverlapCircle(const cocos2d::CCRect& rect, const Circle& c)
{
    struct Polygon polygon(rect);
    
    for(int i = 0; i < (int)polygon.size(); ++i)
    {
        if(isPointInCircle(polygon.getPoint(i), c))
        {
            return true;
        }
    }
    
    return false;
}

// determine if a rect overlaps a polygon
// 判定一个矩形是否和一个多边形有重叠
bool isRectOverlapPolygon(const cocos2d::CCRect& rect, const struct Polygon& p)
{
    struct Polygon polygon(rect);
    
    for(int i = 0; i < (int)polygon.size(); ++i)
    {
        if(isPointInPolygon(polygon.getPoint(i), p))
        {
            return true;
        }
    }
    
    return false;
}

// determine if a point is inside a rectangle, including on the border
// 判定一个点是否在矩形内（包括在边界上）
bool isPointInRect(const cocos2d::CCPoint& p, const cocos2d::CCRect& rect)
{
    return (p.x >= rect.origin.x &&
            p.x <= (rect.origin.x + rect.size.width) &&
            p.y >= rect.origin.y &&
            p.y <= (rect.origin.y + rect.size.height));
}

// determine if a point is inside a circle, including on the border
// 判定一个点是否在圆内（包括在边界上）
bool isPointInCircle(const cocos2d::CCPoint& p, const Circle& c)
{
    return (ccpDistance(p, c.center) <= c.radius);
}

// determine if a point is inside a polygon, including on the border,(way of testing ray)
// 判定一个点是否在多边形内（包括在边界上），射线法
bool isPointInPolygon(const cocos2d::CCPoint& point, const struct Polygon& polygon)
{
    float closeToZero = 0.00001f;
    
    // generate a ray segment from the point toward -x axis
    Segment ray(point, ccp(-1, point.y));    
    
    // calculate the count of intersections between the ray and the polygon
    // if the count is odd, return true, else return false
    int intersectCount = 0;
    
    for(int i = 0; i < (int)polygon.points.size(); ++i)
    {
        Segment s(polygon.points[i], CCPointZero);
        if(i < (int) polygon.points.size() - 1)
        {
            s.end   = polygon.points[i+1];
        }
        else
        {
            s.end   = polygon.points[0];
        }
        
        // check whether the point is on the segment, if yes, return true
        // TODO: maybe we don't need to do that
        
        // if the segment is parallel to x-axis, on this occasion:
        // 1. it's also parallel to the ray, no intersection
        // 2. it overlaps with the ray, ignore
        if( (s.start.y - s.end.y > -closeToZero) && (s.start.y - s.end.y < closeToZero) )
        {
            continue;
        }
        
        // if the ray goes through one vertex of the polygon, 
        // it will produce 2 intersections with the 2 nearby segments connected to that vertex
        // on this occassion, we only count once by ignoring another
        float lowY = MIN(s.start.y, s.end.y);
        if( (point.y - lowY > -closeToZero) && (point.y - lowY < closeToZero) )
        {
            continue;
        }
        
        // if the ray intersects with the segment, with only 1 intersection
        // increase the intersection count by 1
        bool isIntersect = isTwoSegmentsIntersect(ray, s);
        if(isIntersect)
        {
            ++intersectCount;
        }
    }
    
    return ((intersectCount % 2) == 1);
}

// generate a polygon to simulate a fan
// 生成一个多边形来近似一个扇形
void genFanPolygon(const CCPoint& c, float r, float angle, float rotation, struct Polygon& outPolygon)
{
    // define the number of segments on perimeter if we use polygon to simulate a circle
    // 定义用于近似圆周一周的边数
    int NUM_SEGMENTS = 36;
    
    // radian for each segment
    // 每边所覆盖的弧度
    float dR = 2 * M_PI / NUM_SEGMENTS;
    
    // radian of each side of the fan to the x-axis
    // 扇形两边至x轴的弧度角
    float a1 = rotation - angle / 2;
    float a2 = a1 + angle;
    if(a1 < 0 || a2 < 0)
    {
        a1 += 360;
        a2 += 360;
    }
    float radian1 = a1 * M_PI / 180;
    float radian2 = a2 * M_PI / 180;
    
    // number of segments to make up the fan arc
    // 组成扇形弧圈需要的边数
    int numSegments = ceilf((radian2 - radian1) / dR);
    
    // add points
    outPolygon.addPoint(c);
    outPolygon.addPoint(ccp(c.x + r * cosf(radian1), c.y + r * sinf(radian1)));
    for(int i = 0; i < numSegments; ++i)
    {
        float radian    = (i == numSegments - 1) ? radian2 : radian1 + dR * (i+1);
        outPolygon.addPoint(ccp(c.x + r * cosf(radian), c.y + r * sinf(radian)));
    }
}

// rotate a rect
// 旋转矩形
void rotateRect(const cocos2d::CCRect& rect, float angle, const CCPoint& pivot, struct Polygon& outPolygon)
{
    //CCPoint pivot = ccp(rect.origin.x + rect.size.width/2, rect.origin.y + rect.size.height/2);
    CCPoint p1 = rect.origin;
    CCPoint p2 = ccp(p1.x + rect.size.width, p1.y);
    CCPoint p3 = ccp(p2.x, p2.y + rect.size.height);
    CCPoint p4 = ccp(p1.x, p1.y + rect.size.height);
    
    float radian = angle * M_PI / 180;
    p1 = ccpRotateByAngle(p1, pivot, radian);
    p2 = ccpRotateByAngle(p2, pivot, radian);
    p3 = ccpRotateByAngle(p3, pivot, radian);
    p4 = ccpRotateByAngle(p4, pivot, radian);
    
    outPolygon.addPoint(p1);
    outPolygon.addPoint(p2);
    outPolygon.addPoint(p3);
    outPolygon.addPoint(p4);
}