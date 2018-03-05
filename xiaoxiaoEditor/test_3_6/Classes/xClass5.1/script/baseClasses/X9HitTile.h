//
//  X9HitTile.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/10/6.
//
//

#ifndef X9HitTile_hpp
#define X9HitTile_hpp

#include "X9Object.h"

class X9HitTile : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    struct RectData
    {
        int ID;
        Rect rect;
        X9ValueObject* data;
        vector<Vec2> indexs;
        RectData():data(nullptr){}
        RectData(int ID, const Rect& rect, const Vec2& tap, X9ValueObject* data);
        ~RectData();
        void resetIndexs(const Vec2& tap);
    };
    Vec2 tileTap;
    map<int,RectData*> tileMap;
    map<int,map<int,vector<int>>> regedit;
public:
    static X9HitTile* create();
    X9HitTile();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void removed()override;
public:
    void addHitRect(int ID, const Rect& rect, X9ValueObject* data);
    void updateHitRect(int ID, const Rect& rect);
    void removeHitRect(int ID);
    vector<RectData*> hitTest(float x, float y);
};

#endif /* X9HitTile_hpp */
