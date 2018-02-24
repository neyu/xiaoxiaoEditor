//
//  XBezier.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/12/30.
//
//

#include "XBezier.h"
#include "XMath.h"

float XBezier::PRECISION = 1e-10;
XBezier::XBezier():isSegment(false)
{
    
}
XBezier::XBezier(bool isSegment):isSegment(isSegment)
{
    
}
XBezier::XBezier(const Vec2& a,const Vec2& b,const Vec2& c,bool isSegment):isSegment(isSegment)
{
    start.set(a);
    control.set(b);
    end.set(c);
}
XBezier::XBezier(const XBezier& b):XBezier(b.start,b.control,b.end,b.isSegment)
{
}
float XBezier::length()
{
    return getSegmentLength(1);
}
float XBezier::getSegmentLength(float time)
{
    const float csX = control.x - start.x;
    const float csY = control.y - start.y;
    const float nvX = end.x - control.x - csX;
    const float nvY = end.y - control.y - csY;
    
    const float c0 = 4 * (csX * csX + csY * csY);
    const float c1 = 8 * (csX * nvX + csY * nvY);
    const float c2 = 4 * (nvX * nvX + nvY * nvY);
    
    float ft;
    float f0;
    
    if (c2 == 0) {
        if (c1 == 0) {
            ft = sqrt(c0) * time;
            return ft;
        } else {
            ft = (2 / 3) * (c1 * time + c0) * sqrt(c1 * time + c0) / c1;
            f0 = (2 / 3) * c0 * sqrt(c0) / c1;
            return (ft - f0);
        }
    } else {
        const float sqrt_0 = sqrt(c2 * time * time + c1 * time + c0);
        const float sqrt_c0 = sqrt(c0);
        const float sqrt_c2 = sqrt(c2);
        const float exp1 = (0.5 * c1 + c2 * time) / sqrt_c2 + sqrt_0;
        
        if (exp1 < PRECISION) {
            ft = 0.25 * (2 * c2 * time + c1) * sqrt_0 / c2;
        } else {
            ft = 0.25 * (2 * c2 * time + c1) * sqrt_0 / c2 + 0.5 * log((0.5 * c1 + c2 * time) / sqrt_c2 + sqrt_0) / sqrt_c2 * (c0 - 0.25 * c1 * c1 / c2);
        }
        
        const float exp2 = (0.5 * c1) / sqrt_c2 + sqrt_c0;
        if (exp2 < PRECISION) {
            f0 = 0.25 * (c1) * sqrt_c0 / c2;
        } else {
            f0 = 0.25 * (c1) * sqrt_c0 / c2 + 0.5 * log((0.5 * c1) / sqrt_c2 + sqrt_c0) / sqrt_c2 * (c0 - 0.25 * c1 * c1 / c2);
        }
        return ft - f0;
    }
}
float XBezier::getTimeByDistance(float distance)
{
    
    if (isnan(distance)) {
        return 0;
    }
    float arcLength;
    float diffArcLength;
    const float curveLength = length();
    float time = distance / curveLength;
    
    if (isSegment) {
        if (distance <= 0) {
            return 0;
        }
        if (distance >= curveLength) {
            return 1;
        }
    }
    float csX = control.x - start.x;
    float csY = control.y - start.y;
    float ecX = end.x - control.x;
    float ecY = end.y - control.y;
    float nvX = ecX - csX;
    float nvY = ecY - csY;
    
    // vectors: c0 = 4*(cs,cs), с1 = 8*(cs, ec-cs), c2 = 4*(ec-cs,ec-cs)
    float c0 = 4 * (csX * csX + csY * csY);
    float c1 = 8 * (csX * nvX + csY * nvY);
    float c2 = 4 * (nvX * nvX + nvY * nvY);
    
    float c025 = c0 - 0.25 * c1 * c1 / c2;
    float f0Base = 0.25 * c1 * sqrt(c0) / c2;
    float exp2 = 0.5 * c1 / sqrt(c2) + sqrt(c0);
    
    float c00sqrt = sqrt(c0);
    float c20sqrt = sqrt(c2);
    float c22sqrt;
    
    float exp1;
    float ft;
    float ftBase;
    
    float f0;
    int maxIterations = 100;
    
    if (c2 == 0) {
        if (c1 == 0) {
            do {
                arcLength = c00sqrt * time;
                diffArcLength = sqrt(abs((c2 * time * time + c1 * time + c0))) || PRECISION;
                time = time - (arcLength - distance) / diffArcLength;
            } while (abs(arcLength - distance) > PRECISION && maxIterations--);
        } else {
            do {
                arcLength = (2 / 3) * (c1 * time + c0) * sqrt(c1 * time + c0) / c1 - (2 / 3) * c0 * c00sqrt / c1;
                diffArcLength = sqrt(abs((c2 * time * time + c1 * time + c0))) || PRECISION;
                time = time - (arcLength - distance) / diffArcLength;
            } while (abs(arcLength - distance) > PRECISION && maxIterations--);
        }
    } else {
        do {
            c22sqrt = sqrt(abs(c2 * time * time + c1 * time + c0));
            exp1 = (0.5 * c1 + c2 * time) / c20sqrt + c22sqrt;
            ftBase = 0.25 * (2 * c2 * time + c1) * c22sqrt / c2;
            if (exp1 < PRECISION) {
                ft = ftBase;
            } else {
                ft = ftBase + 0.5 * log((0.5 * c1 + c2 * time) / c20sqrt + c22sqrt) / c20sqrt * c025;
            }
            if (exp2 < PRECISION) {
                f0 = f0Base;
            } else {
                f0 = f0Base + 0.5 * log((0.5 * c1) / c20sqrt + c00sqrt) / c20sqrt * c025;
            }
            arcLength = ft - f0;
            diffArcLength = c22sqrt ? c22sqrt : PRECISION;
//            diffArcLength = c22sqrt || PRECISION;
            time = time - (arcLength - distance) / diffArcLength;
        } while (abs(arcLength - distance) > PRECISION && maxIterations--);
    }
    
    return time;
}
Vec2 XBezier::getPoint(float v)
{
    const float n = 1-v;
    return Vec2(start.x*n*n+2*control.x*n*v+end.x*v*v,start.y*n*n+2*control.y*n*v+end.y*v*v);
}
Vec2 XBezier::getPointByDistanceTime(float time)
{
    return getPoint(getTimeByDistance(time*length()));
}
float XBezier::getClosest(const Vec2& fromPoint)
{
    const float sx = start.x;
    const float sy = start.y;
    const float cx = control.x;
    const float cy = control.y;
    const float ex = end.x;
    const float ey = end.y;
    
    const float lpx = sx - fromPoint.x;
    const float lpy = sy - fromPoint.y;
    
    const float kpx = sx - 2 * cx + ex;
    const float kpy = sy - 2 * cy + ey;
    
    const float npx = -2 * sx + 2 * cx;
    const float npy = -2 * sy + 2 * cy;
    
    const float delimiter = 2 * (kpx * kpx + kpy * kpy);
    
    float A;
    float B;
    float C;
    vector<float> extremumTimes;
    
    if(delimiter) {
        A = 3 * (npx * kpx + npy * kpy) / delimiter;
        B = ((npx * npx + npy * npy) + 2 * (lpx * kpx + lpy * kpy)) / delimiter;
        C = (npx * lpx + npy * lpy) / delimiter;
        
        extremumTimes = XMath::solve3xEquation(1, A, B, C);
    } else {
        B = (npx * npx + npy * npy) + 2 * (lpx * kpx + lpy * kpy);
        C = npx * lpx + npy * lpy;
								
        extremumTimes = XMath::solve1xEquation(B, C);
    }
    
    if (isSegment) {
        extremumTimes.push_back(0);
        extremumTimes.push_back(1);
    }
    
    float extremumTime = 0.0f;
    Vec2 extremumPoint;
    float extremumDistance = 0.0f;
    
    float closestPointTime = 0.0f;
    float closestDistance = 0.0f;
    
    bool isInside = false;
    bool isFirst = true;
    const int len = extremumTimes.size();
    for (int i = 0;i < len; i++) {
        extremumTime = extremumTimes[i];
        extremumPoint = getPoint(extremumTime);
        
        extremumDistance = fromPoint.distance(extremumPoint);
        
        isInside = (extremumTime >= 0) && (extremumTime <= 1);
        
        if (extremumDistance < closestDistance || isFirst) {
            if (!isSegment || isInside) {
                isFirst = false;
                closestPointTime = extremumTime;
                closestDistance = extremumDistance;
            }
        }
    }
    return closestPointTime;
}
XBezier XBezier::getSegment(float fromTime, float toTime)
{
    const Vec2 segmentStart = getPoint(fromTime);
    const Vec2 segmentEnd = getPoint(toTime);
    const Vec2 segmentVertex = getPoint((fromTime + toTime) / 2);
    const Vec2 baseMiddle = XMath::mix(segmentStart, segmentEnd, 1 / 2);
    const Vec2 segmentControl = XMath::mix(baseMiddle, segmentVertex, 2);
    return XBezier(segmentStart, segmentControl, segmentEnd, true);
}
float XBezier::getTangentAngle(float time)
{
    Vec2 ab = XMath::mix(start,control,time);
    Vec2 bc = XMath::mix(control,end,time);
    return atan2(ab.y-bc.y, bc.x-ab.x);
}