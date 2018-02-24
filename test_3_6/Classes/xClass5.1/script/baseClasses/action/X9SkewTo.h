//
//  X9SkewTo.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9SkewTo_hpp
#define X9SkewTo_hpp

#include "X9Action.h"

ACT_CLASS_1(X9SkewTo)
Vec2 from;
Vec2 to;
static X9SkewTo* newSkewTo(X9Library* library,float time, float xr, float yr);
ACT_CLASS_2(X9SkewTo)

#endif /* X9SkewTo_hpp */
