//
//  X9RotationTo.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9RotationTo_hpp
#define X9RotationTo_hpp

#include "X9Action.h"

ACT_CLASS_1(X9RotationTo)
float from;
float to;
static X9RotationTo* newRotationTo(X9Library* library,float time, float r);
ACT_CLASS_2(X9RotationTo)

#endif /* X9RotationTo_hpp */
