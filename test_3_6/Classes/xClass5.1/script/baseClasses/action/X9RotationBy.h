//
//  X9RotationBy.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9RotationBy_hpp
#define X9RotationBy_hpp

#include "X9Action.h"

ACT_CLASS_1(X9RotationBy)
float from;
float to;
static X9RotationBy* newRotationBy(X9Library* library,float time, float r);
ACT_CLASS_2(X9RotationBy)

#endif /* X9RotationBy_hpp */
