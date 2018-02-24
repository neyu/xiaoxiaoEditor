//
//  XBezier.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/12/30.
//
//

#ifndef XBezier_hpp
#define XBezier_hpp

#include "csAppClient.h"

class XBezier
{
    static float PRECISION;
public:
    Vec2 start;
    Vec2 control;
    Vec2 end;
    bool isSegment;
    XBezier();
    XBezier(bool isSegment);
    XBezier(const Vec2& a,const Vec2& b,const Vec2& c,bool isSegment);
    XBezier(const XBezier& b);
    
    float length();
    float getSegmentLength(float time);
    float getTimeByDistance(float distance);
    Vec2 getPoint(float time);
    Vec2 getPointByDistanceTime(float time);
    float getClosest(const Vec2& fromPoint);
    XBezier getSegment(float fromTime, float toTime);
    float getTangentAngle(float time);
};

#endif /* XBezier_hpp */
