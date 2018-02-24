//
//  csAppClient.h
//  test
//
//  Created by coolsoft on 15/1/31.
//
//

#ifndef __test__csAppClient__
#define __test__csAppClient__

#define TEST_DEMO 1

#include <vector>
#include <string>
#include <map>
#include <list>
#include <fstream>
#include <set>
#include <iostream>
#include <queue>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <mbstring.h>
#include <tchar.h>
#include <malloc.h>
#include <time.h>
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif


#include "cocos2d.h"
#include "../audio/include/SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "json/document.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace std;


#ifdef WIN32
#define qz_sleep(x) Sleep(x)
#define sprintf sprintf_s
#else
#define qz_sleep(x) usleep(x*1000)
#endif

#define TEST_DEMO 1


#ifdef WIN32
typedef unsigned long DWORD;
#else
typedef unsigned int DWORD;
#endif

#if defined(WIN32) && !defined(DEV_WIN32)
typedef unsigned __int64 UINT64;
typedef signed __int64 INT64;
typedef unsigned __int64 uint64;
typedef signed __int64 int64;
#else
typedef unsigned long long UINT64;
typedef signed long long INT64;
typedef unsigned long long uint64;
typedef signed long long int64;
#endif // defined(WIN32) && !defined(DEV_WIN32)

#if defined(WIN32) && !defined(DEV_WIN32)
//typedef __int64            int64;
typedef __int32            int32;
typedef __int16            int16;
typedef __int8             int8;
//typedef unsigned __int64   uint64;
typedef unsigned __int32   uint32;
typedef unsigned __int16   uint16;
typedef unsigned __int8    uint8;
#else
//typedef __int64_t   int64;
typedef __int32_t   int32;
typedef __int16_t   int16;
typedef __int8_t    int8;
//typedef __uint64_t  uint64;
typedef __uint32_t  uint32;
typedef __uint16_t  uint16;
typedef __uint8_t   uint8;
typedef uint16      WORD;
typedef uint32      DWORD;
#endif

typedef uint64 UL_PTR;
typedef int BOOL;
typedef unsigned char BYTE;


#define GetJsonValue(__J__,__N__,__F__,__V__) \
if(__J__.HasMember(#__N__)){ \
__N__ = __J__[#__N__].Get##__F__(); \
}else{ \
__N__ = __V__; \
}

#define GetJsonValueO(__J__,__N__,__F__) __N__ = __J__[#__N__].Get##__F__();

#define getJsonDocument(_doc, strPath) \
rapidjson::Document _doc; \
unsigned char *pBytes = NULL; \
ssize_t size = 0; \
std::string path = FileUtils::getInstance()->fullPathForFilename(strPath); \
pBytes = FileUtils::getInstance()->getFileData(path, "r", &size); \
std::string load_str((const char*)pBytes, size); \
_doc.Parse<0>(load_str.c_str()); \
delete pBytes;



#endif /* defined(__test__csAppClient__) */
