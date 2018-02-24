//
//  XBezierList.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/12/30.
//
//

#include "XBezierList.h"
#include "XMath.h"
#include "csUtils.h"

XBezierList::XBezierList()
{
    _length = 0;
}
XBezierList::XBezierList(const vector<XBezier>& list)
{
    _list = list;
    reset();
}
void XBezierList::addBezier(const XBezier& b, int index)
{
    if(index < 0 || index == (int)_list.size())
    {
        _list.push_back(b);
        reset();
    }else
    {
        _list.insert(_list.begin()+index, b);
        reset();
    }
}
void XBezierList::removeBezier(int index)
{
    if(index >= 0 && index < _list.size())
    {
        _list.erase(_list.begin()+index);
        reset();
    }
}
void XBezierList::reset()
{
    _length = 0;
    _starts.clear();
    _ends.clear();
    if (!_list.empty()) {
        for(int i = 0;i<_list.size();i++)
        {
            _list[i].isSegment = true;
            _starts.push_back(_length);
            _length += _list[i].length();
            _ends.push_back(_length);
        }
    }
}
float XBezierList::getClosest(const Vec2& p)
{
    float d = MAXFLOAT;
    int idx = -1;
    float t = 0;
    for (int i = 0; i<_list.size(); i++) {
        float _t = _list[i].getClosest(p);
        Vec2 _p = _list[i].getPoint(_t);
        float _d = _p.distance(p);
        if (_t >= 0 && _t <= 1 && _d < d) {
            d = _d;
            t = _t;
            idx = i;
        }
    }
    if (idx == -1) return -1;
    return XMath::mix(_starts[idx], _ends[idx], t)/_length;
}
Vec2 XBezierList::getPoint(float time)
{
    if(time <= 0) return _list.front().start;
    if(time >= 1) return _list.back().end;
    float d = time*_length;
    for(int i = 0;i<_ends.size();i++)
    {
//        _list[i].getTimeByDistance(XMath::umix(_starts[i],_ends[i],d)*_list[i].length())
        if (d < _ends[i]) {
            return _list[i].getPoint(XMath::umix(_starts[i],_ends[i],d));
        }
    }
    CCASSERT(false,"XBezierList::getPoint Error!!!");
}
float XBezierList::getTangentAngle(float time)
{
    if(time <= 0) return _list.front().getTangentAngle(0);
    if(time >= 1) return _list.back().getTangentAngle(1);
    float d = time*_length;
    for(int i = 0;i<_ends.size();i++)
    {
        if (d < _ends[i]) {
            return _list[i].getTangentAngle(XMath::umix(_starts[i],_ends[i],d));
        }
    }
    CCASSERT(false,"XBezierList::getTangentAngle Error!!!");
}
float XBezierList::getSegmentLength(float time)
{
    if (time <= 0) return 0;
    if (time >= 1) return _length;
    float dis = time*_length;
    for (int i = 0; i<_ends.size(); i++) {
        if (dis >= _starts[i] && dis < _ends[i]) {
            float t = XMath::umix(_starts[i], _ends[i], dis);
            return _starts[i]+_list[i].getSegmentLength(t);
        }
    }
    return 0;
}
float XBezierList::getTimeByDistance(float distance)
{
    if (distance <= 0) return 0;
    if (distance >= _length) return 1;
    for (int i = 0; i<_ends.size(); i++) {
        if (distance >= _starts[i] && distance < _ends[i]) {
            return _starts[i]/_length+_list[i].getTimeByDistance(distance-_starts[i])*((_ends[i]-_starts[i])/_length);
        }
    }
    return 0;
}