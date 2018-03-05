//
//  X9RunScript.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/26.
//
//

#ifndef X9RunScript_hpp
#define X9RunScript_hpp

#include "X9ScriptData.h"

class X9RunScript
{
public:
    static map<X9ScriptData::CodeType,std::function<X9ValueObject*(X9RunObject*,X9ScriptData*,X9ScriptClassData*)>> typeCommands;
    static map<string,std::function<X9ValueObject*(X9RunObject*,const vector<X9ValueObject*>&,X9ScriptData*)>> keywordCommands;
    static map<string, std::function<X9ValueObject*(X9RunObject*,X9ValueObject*,X9ValueObject*)>> operatorCommands;
    static void initScript();
    static X9ValueObject* run(X9RunObject* runObj, X9ScriptData* script, X9ScriptClassData* currentClass);
    static void runList(X9RunObject* runObj, const vector<X9ScriptData*> scripts, X9ScriptClassData* currentClass);
};

#endif /* X9RunScript_hpp */
