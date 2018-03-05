//
//  X9SkewBy.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9SkewBy_hpp
#define X9SkewBy_hpp

#include "X9Action.h"

ACT_CLASS_1(X9SkewBy)
Vec2 from;
Vec2 to;
static X9SkewBy* newSkewBy(X9Library* library,float time, float xr, float yr);
ACT_CLASS_2(X9SkewBy)

#endif /* X9SkewBy_hpp */
