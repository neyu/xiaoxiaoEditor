//
//  CSXObjectPool.hpp
//  test
//
//  Created by xiaoxiaozzq on 16/2/22.
//
//

#ifndef CSXObjectPool_hpp
#define CSXObjectPool_hpp

#include "../csAppClient.h"


template<typename T>
class CSXObjectPool
{
    string info;
    int chunkSize;
    std::function<void(T*)> initFunc;
    std::function<void(T*)> removeFunc;
    vector<T*> mFreeObjects;
    vector<T*> mAllObjects;
public:
    bool isLog;
    void allocateChunk()
    {
        T* ts = new T[chunkSize];
        mAllObjects.push_back(ts);
        if(initFunc){
            for(int i = 0;i<chunkSize;i++){
                mFreeObjects.push_back(&ts[i]);
                initFunc(mFreeObjects[i]);
            }
        }else{
            for(int i = 0;i<chunkSize;i++){
                mFreeObjects.push_back(&ts[i]);
            }
        }
    }
    
    CSXObjectPool(const string &info,int chunkSize = 50, std::function<void(T*)> initFunc = nullptr, std::function<void(T*)> removeFunc = nullptr, bool isLog = false):info(info),chunkSize(chunkSize),initFunc(initFunc),removeFunc(removeFunc),isLog(isLog)
    {
        allocateChunk();
    }
    
    ~CSXObjectPool()
    {
        clear();
    }
    
    T* acquireObject()
    {
        if (mFreeObjects.empty()) {
            allocateChunk();
        }
        T* v = mFreeObjects.back();
        mFreeObjects.pop_back();
//        if(isLog) log("ObjectPool>>>> %s acquire num:%d",info.c_str(),(int)(mAllObjects.size()*chunkSize-mFreeObjects.size()));
        return v;
    }
    
    void releaseObject(T* obj)
    {
        if(removeFunc){
            removeFunc(obj);
        }
        mFreeObjects.push_back(obj);
//        if(isLog) log("ObjectPool>>>> %s release num:%d",info.c_str(),(int)(mAllObjects.size()*chunkSize-mFreeObjects.size()));
    }
    
    void clear()
    {
        for(int i = 0;i<mAllObjects.size();i++)
        {
            delete [] mAllObjects[i];
        }
    }
};

#endif /* CSXObjectPool_hpp */
