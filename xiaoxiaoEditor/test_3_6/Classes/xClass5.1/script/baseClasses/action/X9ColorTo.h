//
//  X9ColorTo.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9ColorTo_hpp
#define X9ColorTo_hpp
#include "X9Action.h"

ACT_CLASS_1(X9ColorTo)
Color3B from;
Color3B to;
static X9ColorTo* newColorTo(X9Library* library,float time, float R, float G, float B);
ACT_CLASS_2(X9ColorTo)

#endif /* X9ColorTo_hpp */
