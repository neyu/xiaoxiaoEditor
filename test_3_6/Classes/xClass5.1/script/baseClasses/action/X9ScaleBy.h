//
//  X9ScaleBy.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9ScaleBy_hpp
#define X9ScaleBy_hpp

#include "X9Action.h"
ACT_CLASS_1(X9ScaleBy)
Vec2 from;
Vec2 to;
static X9ScaleBy* newScaleBy(X9Library* library,float time, float x, float y);
ACT_CLASS_2(X9ScaleBy)

#endif /* X9ScaleBy_hpp */
