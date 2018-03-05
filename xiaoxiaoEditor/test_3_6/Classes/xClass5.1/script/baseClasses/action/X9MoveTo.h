//
//  X9MoveTo.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9MoveTo_hpp
#define X9MoveTo_hpp

#include "X9Action.h"

ACT_CLASS_1(X9MoveTo)
Vec2 from;
Vec2 to;
static X9MoveTo* newMoveTo(X9Library* library,float time, float x, float y);
ACT_CLASS_2(X9MoveTo)


#endif /* X9MoveTo_hpp */
