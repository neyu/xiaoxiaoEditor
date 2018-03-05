//
//  XUtils.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 16/8/14.
//
//

#ifndef XUtils_hpp
#define XUtils_hpp

#include "csAppClient.h"


#define RANDOMF     (float)rand() / (float)(RAND_MAX)


inline float modx(float a, float b)
{
    return a-b*floor(a/b);
}
inline float mix(float a, float b, float v)
{
    return a+(b-a)*v;
}
inline Vec2 mix(const Vec2& a, const Vec2& b, float v)
{
    return Vec2(mix(a.x,b.x,v),mix(a.y,b.y,v));
}
inline float mix2x(float a, float b, float c, float v)
{
    const float n = 1-v;
    return a*n*n+2*b*n*v+c*v*v;
}
inline Vec2 mix2x(const Vec2& a, const Vec2& b, const Vec2& c, float v)
{
    return Vec2(mix2x(a.x, b.x, c.x, v),mix2x(a.y, b.y, c.y, v));
}
inline float mix3x(float a, float b, float c, float d, float v)
{
    const float n = 1-v;
    return a*n*n*n+3*b*n*n*v+3*c*n*v*v+d*v*v*v;
}
inline Vec2 mix3x(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, float v)
{
    return Vec2(mix3x(a.x, b.x, c.x, d.x, v),mix3x(a.y, b.y, c.y, d.y, v));
}
//inline float mix(float a,float b,float c,float v)
//{
//    float n = 1.0-v;
//    return a*n*n+2*b*n*v+c*v*v;
//}
//inline float mix(float a,float b,float c,float d,float v)
//{
//    float n = 1.0-v;
//    return a*n*n*n+3*b*n*n*v+3*c*n*v*v+d*v*v*v;
//}
//inline vector<float> umix(float a, float b, float c, float f)
//{
//    return solve2xEquation(a-2*b+c, 2*(b-a), a-f);
//}
inline float umix(float a, float b, float f)
{
    if(a == f || a == b) return 0;
    return (f-a)/(b-a);
}
inline Vec2 mixUV(const Vec2& A,const Vec2& U,const Vec2& V,const Vec2& uv)
{
    //P=(1-u-v)A+uC+vB
    float s = 1.0f-uv.x-uv.y;
    float x = s*A.x+uv.x*U.x+uv.y*V.x;
    float y = s*A.y+uv.x*U.y+uv.y*V.y;
    return Vec2(x,y);
}
inline Vec2 umixUV(const Vec2& A,const Vec2& U,const Vec2& V,const Vec2& P)
{
    if(A == P) return Vec2::ZERO;
    const Vec2 v0(U-A);
    const Vec2 v1(V-A);
    const Vec2 v2(P-A);
    
    const float v = (v2.x*v0.y-v2.y*v0.x)/(v1.x*v0.y-v1.y*v0.x);
    const float u = (v2.x-v*v1.x)/v0.x;
    return Vec2(u,v);
}
inline float radianLength(float from, float to, bool isAbs = false)
{
    if (isAbs) {
        return modx(to-from+M_PI*256,M_PI*2);
    }
    return modx(to-from+M_PI*257,M_PI*2)-M_PI;
}
inline float rotationLength(float from, float to, bool isAbs = false)
{
    if (isAbs) {
        return modx(to-from+180*256,360);
    }
    return modx(to-from+180*257,360)-180;
}
inline bool isInTriangle(const Vec2& A,const Vec2& B,const Vec2& C,const Vec2& P)
{
    const Vec2 v0(C-A);
    const Vec2 v1(B-A);
    const Vec2 v2(P-A);
    const float v = (v2.x*v0.y-v2.y*v0.x)/(v1.x*v0.y-v1.y*v0.x);
    if (v < 0 || v > 1) {
        return false;
    }
    const float u = (v2.x-v*v1.x)/v0.x;
    if (u < 0 || u > 1) {
        return false;
    }
    return u+v <= 1;
}

inline float beelineIntersectPoint(const Vec2& P1, const Vec2& P2, const Vec2& Q1, const Vec2& Q2)
{
    float x;
    float y;
    if (P1.y == P2.y)
    {
        if(Q1.y == Q2.y) return -100;
        y = P1.y;
        if(Q2.x == Q1.x)
        {
            x = Q1.x;
        }else
        {
            x = (P1.y - Q1.y) / (Q2.y - Q1.y) * (Q2.x - Q1.x) + Q1.x;
        }
        return umix(P1.x, P2.x, x);
    }
    if (Q1.y == Q2.y)
    {
        if(P1.y == P2.y) return -100;
        y = Q1.y;
        if(P2.x == P1.x)
        {
            x = P1.x;
        }else
        {
            x = (Q1.y - P1.y) / (P2.y - P1.y) * (P2.x - P1.x) + P1.x;
        }
        return umix(P1.x, P2.x, x);
    }
    //
    if (P1.x == P2.x)
    {
        if(Q1.x == Q2.x) return -100;
        x = P1.x;
        if(Q2.y == Q1.y)
        {
            y = Q1.y;
        }else
        {
            y = (P1.x - Q1.x) / (Q2.x - Q1.x) * (Q2.y - Q1.y) + Q1.y;
        }
        return umix(P1.y, P2.y, y);
    }
    if (Q1.x == Q2.x)
    {
        if(P1.x == P2.x) return -100;
        x = Q1.x;
        if(P2.y == P1.y)
        {
            y = P1.y;
        }else
        {
            y = (Q1.x - P1.x) / (P2.x - P1.x) * (P2.y - P1.y) + P1.y;
        }
        return umix(P1.y, P2.y, y);
    }
    float k1 = (P1.y - P2.y) / (P1.x - P2.x);
    float n1 = P1.y / k1 - P1.x;
    float k2 = (Q1.y - Q2.y) / (Q1.x - Q2.x);
    float n2 = Q1.y / k2 - Q1.x;
    float xx = (k2 * n2 - k1 * n1) / (k1 - k2);
    return umix(P1.x, P2.x, xx);
}

////////////
inline float randomf()
{
    return RANDOMF;
}
inline float randomAt(float a, float b)
{
    return randomf()*(b-a)+a;
}
inline int randomAt(int a, int b)
{
    return randomf()*(b-a+1)+a;
}

inline vector<string> splitStr(string str,string pattern)
{
    string::size_type pos;
    vector<string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}
inline void anglePoint(Vec2 &p, float cos, float sin, const Vec2 &axes)
{
    float ax = (p.x-axes.x)*cos-(p.y-axes.y)*sin;
    p.y = (p.y-axes.y)*cos+(p.x-axes.x)*sin+axes.y;
    p.x = ax+axes.x;
}










//float PRECISION = 1e-10;
inline float mathPower(float x, float p) {
    if (x > 0) {
        return exp(log(x)*p);
    } else if (x < 0) {
        return -exp(log(-x)*p);
    }
    return 0;
}

inline float mathAtan2(float dx, float dy) {
    float a;
    if (dx == 0) {
        a = M_PI_2;
    } else if (dx > 0) {
        a = atan(abs(dy/dx));
    } else {
        a = M_PI - atan(abs(dy/dx));
    }
    if (dy >= 0) {
        return a;
    }
    return -a;
}

/**
 * ax + b = 0
 **/
inline vector<float> solve1xEquation(float A, float B) {
    if (abs(A) < 1e-10) {
        return vector<float>();
    }
    return {-B/A};
}

/**
 * ax^2 + bx + c = 0
 **/
inline vector<float> solve2xEquation(float A, float B, float C) {
    
    if (abs(A) < 1e-10) {
        return solve1xEquation(B, C);
    }
    float b = B/A;
    float c = C/A;
    
    if (abs(b) < 1e-10) {
        b = 0;
    }
    if (abs(c) < 1e-10) {
        c = 0;
    }
    
    float d = b*b - 4.0f*c;
    
    if (abs(d) < 1e-10) {
        d = 0;
    }
    if (d > 0) {
        d = sqrt(d);
        return {(-b - d)/2.0f, (-b + d)/2.0f};
    }
    if (d == 0) {
        return {-b/2.0f};
    }
    return vector<float>();
}

/**
 * ax^3 + bx^2 + cx + d = 0
 **/
inline vector<float> solve3xEquation(float A, float B, float C, float D) {
    if (abs(A) < 1e-10) {
        return solve2xEquation(B, C, D);
    }
    
    float b = B/A;
    float c = C/A;
    float d = D/A;
    
    
    if (abs(b) < 1e-10) {
        b = 0;
    }
    if (abs(c) < 1e-10) {
        c = 0;
    }
    if (abs(d) < 1e-10) {
        d = 0;
    }
    
    const float p = -b*b/3.0f + c;
    const float q = 2.0f*b*b*b/27.0f - b*c/3.0f + d;
    const float r = q*q/4.0f + p*p*p/27.0f;
    
    float x;
    if (r >= 0) {
        const float squrtR = sqrt(r);
        x = mathPower(-q/2.0f + squrtR, 1.0f/3.0f) + mathPower(-q/2.0f - squrtR, 1.0f/3.0f) - b/3.0f;
    } else {
        x = 2*sqrt(-p/3.0f)*cos(mathAtan2(-q/2.0f, sqrt(abs(r)))/3.0f) - b/3.0f;
    }
    
    vector<float> quadraticSolve = solve2xEquation(1.0f, x + b, x*x + b*x + c);
    if ((quadraticSolve[0] != x) && (quadraticSolve[1] != x)) {
        quadraticSolve.push_back(x);
    }
    
    return quadraticSolve;
}

/**
 * ax^4 + bx^3 + cx^2 + dx + e = 0
 **/

vector<float> solve4xEquation(float A, float B, float C, float D, float E) {
    
    if (abs(A) < 1e-10) {
        return solve3xEquation(B, C, D, E);
    }
    
    float b = B/A;
    float c = C/A;
    float d = D/A;
    float e = E/A;
    
    if (abs(b) < 1e-10) {
        b = 0;
    }
    if (abs(c) < 1e-10) {
        c = 0;
    }
    if (abs(d) < 1e-10) {
        d = 0;
    }
    if (abs(e) < 1e-10) {
        e = 0;
    }
    
    float c1;
    float c2;
    
    const float cubicSolve = solve3xEquation(1, -c, b*d - 4*e, -b*b*e + 4*c*e - d*d)[0];
    const float cubicSolve2 = cubicSolve/2;
    
    
    float m = b*b/4 - c + cubicSolve;
    float n = b*cubicSolve2 - d;
    float k = cubicSolve2*cubicSolve2 - e;
    
    if (abs(m) < 1e-10) {
        m = 0;
    }
    if (abs(n) < 1e-10) {
        n = 0;
    }
    if (abs(k) < 1e-10) {
        k = 0;
    }
    
    
    if ((m >= 0) && (k >= 0)) {
        const float sqrtK = sqrt(k);
        const float sqrtM = sqrt(m);
        
        c1 = b/2 - sqrtM;
        if (n > 0) {
            c2 = cubicSolve2 - sqrtK;
        } else {
            c2 = cubicSolve2 + sqrtK;
        }
        
        vector<float> quarticSolve = solve2xEquation(1, c1, c2);
        c1 = b/2 + sqrtM;
        if (n > 0) {
            c2 = cubicSolve2 + sqrtK;
        } else {
            c2 = cubicSolve2 - sqrtK;
        }
        vector<float> temp = solve2xEquation(1, c1, c2);
        for (int i = 0; i<temp.size(); i++) {
            quarticSolve.push_back(temp[i]);
        }
        return quarticSolve;
    }
    return vector<float>();
}



//const float PRECISION = 1e-10;
////const float 
//
//
//inline float mathPower(float x, float p) {
//    if (x > 0) {
//        return exp(log(x)*p);
//    } else if (x < 0) {
//        return -exp(log(-x)*p);
//    } else {
//        return 0;
//    }
//}
//
//inline float mathAtan2(float dx, float dy) {
//    float a;
//    if (dx == 0) {
//        a = M_PI/2;
//    } else if (dx > 0) {
//        a = atan(abs(dy/dx));
//    } else {
//        a = M_PI - atan(abs(dy/dx));
//    }
//    if (dy >= 0) {
//        return a;
//    } else {
//        return -a;
//    }
//}
///**
// * ax + b = 0
// **/
//inline vector<float> solveLinearEquation(float A, float B){
//    vector<float> list;
//    if (abs(A) < PRECISION) {
//        list.push_back(-B/A);
//    }
//    return list;
//}
//
///**
// * ax^2 + bx + c = 0
// **/
//inline vector<float> solveQuadraticEquation(float A, float B, float C) {
//    
//    if (abs(A) < PRECISION) {
//        return solveLinearEquation(B, C);
//    }
//    float b = B/A;
//    float c = C/A;
//    
//    if (abs(b) < PRECISION) {
//        b = 0;
//    }
//    if (abs(c) < PRECISION) {
//        c = 0;
//    }
//    
//    float d = b*b - 4*c;
//    
//    if (abs(d) < PRECISION) {
//        d = 0;
//    }
//    vector<float> list;
//    if (d > 0) {
//        d = sqrt(d);
//        list.push_back((-b - d)/2);
//        list.push_back((-b + d)/2);
//        return list;
//    }
//    if (d == 0) {
//        list.push_back(-b/2);
//        return list;
//    }
//    return list;
//}
//
///**
// * ax^3 + bx^2 + cx + d = 0
// **/
//inline vector<float> solveCubicEquation(float A, float B, float C, float D) {
//    if (abs(A) < PRECISION) {
//        return solveQuadraticEquation(B, C, D);
//    }
//    
//    float b = B/A;
//    float c = C/A;
//    float d = D/A;
//    
//    
//    if (abs(b) < PRECISION) {
//        b = 0;
//    }
//    if (abs(c) < PRECISION) {
//        c = 0;
//    }
//    if (abs(d) < PRECISION) {
//        d = 0;
//    }
//    
//    const float p = -b*b/3 + c;
//    const float q = 2*b*b*b/27 - b*c/3 + d;
//    const float r = q*q/4 + p*p*p/27;
//    
//    float x;
//    if (r >= 0) {
//        const float squrtR = sqrt(r);
//        x = mathPower(-q/2 + squrtR, 1/3) + mathPower(-q/2 - squrtR, 1/3) - b/3;
//    } else {
//        x = 2*sqrt(-p/3)*cosf(mathAtan2(-q/2, sqrt(abs(r)))/3) - b/3;
//    }
//    
//    vector<float> quadraticSolve = solveQuadraticEquation(1, x + b, x*x + b*x + c);
//    if ((quadraticSolve[0] != x) && (quadraticSolve[1] != x)) {
//        quadraticSolve.push_back(x);
//    }
//    
//    return quadraticSolve;
//}
//
///**
// * ax^4 + bx^3 + cx^2 + dx + e = 0
// **/
//
//inline vector<float> solveQuarticEquation(float A, float B, float C, float D, float E) {
//    
//    if (abs(A) < PRECISION) {
//        return solveCubicEquation(B, C, D, E);
//    }
//    
//    float b = B/A;
//    float c = C/A;
//    float d = D/A;
//    float e = E/A;
//    
//    if (abs(b) < PRECISION) {
//        b = 0;
//    }
//    if (abs(c) < PRECISION) {
//        c = 0;
//    }
//    if (abs(d) < PRECISION) {
//        d = 0;
//    }
//    if (abs(e) < PRECISION) {
//        e = 0;
//    }
//    
//    float c1;
//    float c2;
//    
//    const float cubicSolve = solveCubicEquation(1, -c, b*d - 4*e, -b*b*e + 4*c*e - d*d)[0];
//    const float cubicSolve2 = cubicSolve/2;
//    
//    
//    float m = b*b/4 - c + cubicSolve;
//    float n = b*cubicSolve2 - d;
//    float k = cubicSolve2*cubicSolve2 - e;
//    
//    if (abs(m) < PRECISION) {
//        m = 0;
//    }
//    if (abs(n) < PRECISION) {
//        n = 0;
//    }
//    if (abs(k) < PRECISION) {
//        k = 0;
//    }
//    
//    
//    if ((m >= 0) && (k >= 0)) {
//        const float sqrtK = sqrt(k);
//        const float sqrtM = sqrt(m);
//        
//        c1 = b/2 - sqrtM;
//        if (n > 0) {
//            c2 = cubicSolve2 - sqrtK;
//        } else {
//            c2 = cubicSolve2 + sqrtK;
//        }
//        
//        vector<float> quarticSolve = solveQuadraticEquation(1, c1, c2);
//        c1 = b/2 + sqrtM;
//        if (n > 0) {
//            c2 = cubicSolve2 + sqrtK;
//        } else {
//            c2 = cubicSolve2 - sqrtK;
//        }
//        vector<float> temp = solveQuadraticEquation(1, c1, c2);
//        for (int i = 0; i<temp.size(); i++) {
//            quarticSolve.push_back(temp[i]);
//        }
//        return quarticSolve;
//    }
//    return vector<float>();
//}






#endif /* XUtils_hpp */
