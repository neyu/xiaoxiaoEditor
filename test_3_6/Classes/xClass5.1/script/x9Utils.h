//
//  x9Math.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/8.
//
//
#pragma once

#ifndef x9Utils_hpp
#define x9Utils_hpp

#include "x9AppClient.h"
//namespace x9Utils {
inline float mod(float a, float b)
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
        return mod(to-from+M_PI*256,M_PI*2);
    }
    return mod(to-from+M_PI*257,M_PI*2)-M_PI;
}
inline float rotationLength(float from, float to, bool isAbs = false)
{
    if (isAbs) {
        return mod(to-from+180*256,360);
    }
    return mod(to-from+180*257,360)-180;
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
//}
#endif /* x9Math_hpp */
