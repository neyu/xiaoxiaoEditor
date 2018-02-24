//
//  X9VisibleTo.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9VisibleTo_hpp
#define X9VisibleTo_hpp

#include "X9Action.h"

ACT_CLASS_1(X9VisibleTo)
bool to;
static X9VisibleTo* newVisibleTo(X9Library* library,float time, bool value);
ACT_CLASS_2(X9VisibleTo)

#endif /* X9VisibleTo_hpp */
