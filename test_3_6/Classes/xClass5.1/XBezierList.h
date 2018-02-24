//
//  XBezierList.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/12/30.
//
//

#ifndef XBezierList_hpp
#define XBezierList_hpp

#include "XBezier.h"

class XBezierList
{
    float _length;
    vector<float> _starts;
    vector<float> _ends;
public:
    vector<XBezier> _list;
    XBezierList();
    XBezierList(const vector<XBezier>& list);
    void addBezier(const XBezier& b, int index = -1);
    void removeBezier(int index);
    void reset();
    float length(){return _length;};
    float getClosest(const Vec2& p);
    Vec2 getPoint(float time);
    float getTangentAngle(float time);
    float getSegmentLength(float time);
    float getTimeByDistance(float distance);
};

#endif /* XBezierList_hpp */
