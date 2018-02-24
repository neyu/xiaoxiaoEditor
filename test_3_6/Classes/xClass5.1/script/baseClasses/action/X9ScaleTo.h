//
//  X9ScaleTo.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9ScaleTo_hpp
#define X9ScaleTo_hpp

#include "X9Action.h"

ACT_CLASS_1(X9ScaleTo)
Vec2 from;
Vec2 to;
static X9ScaleTo* newScaleTo(X9Library* library,float time, float x, float y);
ACT_CLASS_2(X9ScaleTo)

#endif /* X9ScaleTo_hpp */
