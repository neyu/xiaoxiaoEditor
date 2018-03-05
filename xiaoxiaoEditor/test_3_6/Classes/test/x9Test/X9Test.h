//
//  X9Test.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/5.
//
//

#ifndef X9Test_hpp
#define X9Test_hpp

#include "../BaseTestLayer.h"
#include "../../xClass5.1/script/X9Player.h"

class X9Test : public BaseTestLayer
{
public:
    NEW_FUNC(X9Test);
    virtual void onEnter()override;
    vector<string> getFilePathAtVec(string filePath);
};

class X9Test_script1 : public BaseTestItemLayer
{
public:
    virtual void onEnter()override;
    virtual void update(float delay)override;
};
class X9Test_script2 : public BaseTestItemLayer
{
    X9Player* player;
    X9Value* test(X9Value* value);
public:
    virtual void onEnter()override;
    virtual void onTouchesBegan(const std::vector<Touch*>& touchs, Event* event) override;
    virtual void onTouchesMoved(const std::vector<Touch*>& touchs, Event* event) override;
    virtual void onTouchesEnded(const std::vector<Touch*>& touchs, Event* event) override;
    virtual void onTouchesCancelled(const std::vector<Touch*>& touchs, Event* event) override;
};

#endif /* X9Test_hpp */
