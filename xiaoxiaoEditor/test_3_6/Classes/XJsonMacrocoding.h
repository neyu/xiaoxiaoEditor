//
//  XJsonMacrocoding.h
//  test_3_6
//
//  Created by xiaoxiaozzq on 16/8/3.
//
//

#pragma once

#ifndef XJsonMacrocoding_h
#define XJsonMacrocoding_h

#define loadJsonDoc(__DOC__,__NAME__) \
rapidjson::Document __DOC__; \
unsigned char *__pBytes__ = NULL; \
ssize_t __size__ = 0; \
std::string __path__ = FileUtils::getInstance()->fullPathForFilename(__NAME__); \
__pBytes__ = FileUtils::getInstance()->getFileData(__path__, "r", &__size__); \
std::string __load_str__((const char*)__pBytes__, __size__); \
__DOC__.Parse<0>(__load_str__.c_str()); \
delete __pBytes__;

#define __jsonValueKey(__A__,__DK__,__DOC__,_T_) \
__A__ = __DOC__[#__DK__].Get##_T_()

#define __jsonValue(__A__,__DOC__,_T_) __jsonValueKey(__A__,__A__,__DOC__,_T_)

#define __jsonValueByArray(__A__,__DOC__,_T_) \
if(__DOC__.HasMember(#__A__)){ \
__A__.resize(__DOC__[#__A__].Size()); \
for(int i = 0;i<__A__.size();i++) __A__[i] = __DOC__[#__A__][i].Get##_T_(); \
}

#define __defaultValue(__A__,__DOC__,__D__,_T_) \
__A__ = __DOC__.HasMember(#__A__) ? __DOC__[#__A__].Get##_T_() : __D__

#define jsonInt(__A__,__DOC__) __jsonValue(__A__,__DOC__,Int)
#define jsonFloat(__A__,__DOC__) __jsonValue(__A__,__DOC__,Double)
#define jsonString(__A__,__DOC__) __jsonValue(__A__,__DOC__,String)
#define jsonBool(__A__,__DOC__) __jsonValue(__A__,__DOC__,Bool)

#define jsonKInt(__A__,__K__,__DOC__) __jsonValueKey(__A__,__K__,__DOC__,Int)
#define jsonKFloat(__A__,__K__,__DOC__) __jsonValueKey(__A__,__K__,__DOC__,Double)
#define jsonKString(__A__,__K__,__DOC__) __jsonValueKey(__A__,__K__,__DOC__,String)
#define jsonKBool(__A__,__K__,__DOC__) __jsonValueKey(__A__,__K__,__DOC__,Bool)

#define jsonIntByArr(__A__,__DOC__) __jsonValueByArray(__A__,__DOC__,Int)
#define jsonFloatByArr(__A__,__DOC__) __jsonValueByArray(__A__,__DOC__,Double)
#define jsonStringByArr(__A__,__DOC__) __jsonValueByArray(__A__,__DOC__,String)
#define jsonBoolByArr(__A__,__DOC__) __jsonValueByArray(__A__,__DOC__,Bool)

#define defaultInt(__A__,__DOC__,__D__) __defaultValue(__A__,__DOC__,__D__,Int)
#define defaultFloat(__A__,__DOC__,__D__) __defaultValue(__A__,__DOC__,__D__,Double)
#define defaultString(__A__,__DOC__,__D__) __defaultValue(__A__,__DOC__,__D__,String)
#define defaultBool(__A__,__DOC__,__D__) __defaultValue(__A__,__DOC__,__D__,Bool)

#define defaultFloatColor(_A_,_K_,_DOC_,_D_) \
_A_ = _DOC_.HasMember(#_K_) ? _DOC_[#_K_].GetDouble() : _D_
#define defaultIntColor(_A_,_K_,_DOC_,_D_) \
_A_ = _DOC_.HasMember(#_K_) ? _DOC_[#_K_].GetInt() : _D_

#define jsonObjByArray(__A__,__DOC__,_T_,...) \
if(__DOC__.HasMember(#__A__)){ \
    for (int i = 0; i<__DOC__[#__A__].Size(); i++) { \
        __A__.push_back(_T_(__DOC__[#__A__][i],##__VA_ARGS__)); \
    } \
}



#endif /* XJsonMacrocoding_h */
