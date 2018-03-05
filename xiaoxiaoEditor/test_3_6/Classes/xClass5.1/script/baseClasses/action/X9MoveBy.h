//
//  X9MoveBy.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9MoveBy_hpp
#define X9MoveBy_hpp

#include "X9Action.h"
ACT_CLASS_1(X9MoveBy)
Vec2 from;
Vec2 to;
static X9MoveBy* newMoveBy(X9Library* library,float time, float x, float y);
ACT_CLASS_2(X9MoveBy)


#endif /* X9MoveBy_hpp */
