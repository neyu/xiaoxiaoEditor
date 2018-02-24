//
//  X9AlphaTo.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9AlphaTo_hpp
#define X9AlphaTo_hpp

#include "X9Action.h"

ACT_CLASS_1(X9AlphaTo)
float from;
float to;
static X9AlphaTo* newAlphaTo(X9Library* library,float time, float A);
ACT_CLASS_2(X9AlphaTo)

#endif /* X9AlphaTo_hpp */
