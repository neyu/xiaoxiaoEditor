//
//  X9HitTile.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/10/6.
//
//

#include "X9HitTile.h"
#include "X9Array.h"
#include "X9Rect.h"
#include "X9Point.h"

X9ValueObject* base_hitTile_addHitRect(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() >= 2 && values.size() <= 3 && values[0]->isNumber() && values[0]->getNumber() >= 0 && values[1]->isObject<X9Rect*>(),"addHitRect Error!!!");
    X9HitTile* tile = dynamic_cast<X9HitTile*>(target);
    X9ValueObject* data = nullptr;
    if (values.size() == 3) {
        data = values[2]->clone();
    }
    tile->addHitRect(values[0]->getNumber(), values[1]->getObject<X9Rect*>()->rect, data);
    return nullptr;
}
X9ValueObject* base_hitTile_updateHitRect(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->getNumber() >= 0 && values[1]->isObject<X9Rect*>(),"addHitRect Error!!!");
    X9HitTile* tile = dynamic_cast<X9HitTile*>(target);
    tile->updateHitRect(values[0]->getNumber(), values[1]->getObject<X9Rect*>()->rect);
    return nullptr;
}
X9ValueObject* base_hitTile_removeHitRect(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"removeHitRect Error!!!");
    X9HitTile* tile = dynamic_cast<X9HitTile*>(target);
    tile->removeHitRect(values[0]->getNumber());
    return nullptr;
}
X9ValueObject* base_hitTile_hitTest(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() >= 1 && values.size() <= 2,"hitTest Error!!!");
    X9HitTile* tile = dynamic_cast<X9HitTile*>(target);
    float ax;
    float ay;
    if (values.size() == 1) {
        X9ASSERT(values[0]->isObject<X9Point*>(),"hitTest Error!!!");
        ax = values[0]->getObject<X9Point*>()->point.x;
        ay = values[0]->getObject<X9Point*>()->point.y;
    }else{
        X9ASSERT(values[0]->isNumber() && values[1]->isNumber(),"hitTest Error!!!");
        ax = values[0]->getNumber();
        ay = values[1]->getNumber();
    }
    vector<X9HitTile::RectData*> zu = tile->hitTest(ax,ay);
    X9Array* arr = X9Array::newArray(target->getLibrary());
    for (int i = 0; i<zu.size(); i++) {
        X9Object* obj = X9Object::newObject(target->getLibrary());
        obj->addNumberValue("ID", zu[i]->ID);
        obj->addObjectValue("rect", X9Rect::newRect(target->getLibrary(),zu[i]->rect.origin.x,zu[i]->rect.origin.y,zu[i]->rect.size.width,zu[i]->rect.size.height));
        obj->setValue(X9RunObject::MemberType::MT_PROPERTY, "data", zu[i]->data->clone());
        arr->pushObject(obj);
    }
    return X9ValueObject::createWithObject(arr);
}

X9HitTile::RectData::RectData(int ID, const Rect& rect, const Vec2& tap, X9ValueObject* data):ID(ID),rect(rect),data(data)
{
    if (this->data) {
        this->data->use();
    }
    resetIndexs(tap);
}
X9HitTile::RectData::~RectData()
{
    if (data) {
        data->nonuse();
    }
}
void X9HitTile::RectData::resetIndexs(const Vec2& tap)
{
    indexs.clear();
    int stX = floor(rect.getMinX()/tap.x);
    int stY = floor(rect.getMinY()/tap.y);
    int edX = ceil(rect.getMaxX()/tap.x);
    int edY = ceil(rect.getMaxY()/tap.y);
    for (int i = stX; i< edX; i++) {
        for (int k = stY; k<edY; k++) {
            indexs.push_back(Vec2(i,k));
        }
    }
}


void X9HitTile::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(hitTile_,addHitRect);
    x9_AddBaseFunc(hitTile_,updateHitRect);
    x9_AddBaseFunc(hitTile_,removeHitRect);
    x9_AddBaseFunc(hitTile_,hitTest);
//
//    x9_AddBaseGet(userDefault_,data);
//    x9_AddBaseSGet(userDefault_,isAutoSave);
}
void X9HitTile::setConstValues(X9ScriptClassData* classData)
{
    
}
X9HitTile* X9HitTile::create()
{
    CREATE_NEW_AUTO_OBJ(X9HitTile,);
}
X9HitTile::X9HitTile():X9Object("X9HitTile")
{
    x9_setCtor(HitTile);
}

void X9HitTile::initObject(const vector<X9ValueObject*>& vs)
{
    //tapW, tapH
    X9ASSERT(vs.size() == 2 && vs[0]->isNumber() && vs[1]->isNumber(),"new HitTile Error!!!");
    tileTap.x = vs[0]->getNumber();
    tileTap.y = vs[1]->getNumber();
}
void X9HitTile::removed()
{
    for (auto it = tileMap.begin(); it!=tileMap.end(); it++) {
        delete it->second;
    }
    tileMap.clear();
    regedit.clear();
    X9Object::removed();
}
void X9HitTile::addHitRect(int ID, const Rect& rect, X9ValueObject* data)
{
    CCASSERT(tileMap.find(ID) == tileMap.end(),"addHitRect Error!!!");
    tileMap[ID] = new RectData(ID,rect,tileTap, data);
    for (int i = 0; i<tileMap[ID]->indexs.size(); i++) {
        int x = tileMap[ID]->indexs[i].x;
        int y = tileMap[ID]->indexs[i].y;
        if (regedit.find(x) == regedit.end()) {
            regedit[x] = map<int,vector<int>>();
        }
        if (regedit[x].find(y) == regedit[x].end()) {
            regedit[x][y] = vector<int>();
        }
        regedit[x][y].push_back(ID);
    }
}
void X9HitTile::updateHitRect(int ID, const Rect& rect)
{
    CCASSERT(tileMap.find(ID) != tileMap.end(),"updateHitRect Error!!!");
    for(int i = 0;i<tileMap[ID]->indexs.size();i++)
    {
        int x = tileMap[ID]->indexs[i].x;
        int y = tileMap[ID]->indexs[i].y;
        for (auto it = regedit[x][y].begin(); it != regedit[x][y].end(); it++) {
            if ((*it) == ID) {
                regedit[x][y].erase(it);
                if (regedit[x][y].empty()) {
                    regedit[x].erase(y);
                }
                break;
            }
        }
    }
    tileMap[ID]->rect = rect;
    tileMap[ID]->resetIndexs(tileTap);
    
    for (int i = 0; i<tileMap[ID]->indexs.size(); i++) {
        int x = tileMap[ID]->indexs[i].x;
        int y = tileMap[ID]->indexs[i].y;
        if (regedit.find(x) == regedit.end()) {
            regedit[x] = map<int,vector<int>>();
        }
        if (regedit[x].find(y) == regedit[x].end()) {
            regedit[x][y] = vector<int>();
        }
        regedit[x][y].push_back(ID);
    }
}
void X9HitTile::removeHitRect(int ID)
{
    if (tileMap.find(ID) != tileMap.end()) {
        for(int i = 0;i<tileMap[ID]->indexs.size();i++)
        {
            int x = tileMap[ID]->indexs[i].x;
            int y = tileMap[ID]->indexs[i].y;
            for (auto it = regedit[x][y].begin(); it != regedit[x][y].end(); it++) {
                if ((*it) == ID) {
                    regedit[x][y].erase(it);
                    if (regedit[x][y].empty()) {
                        regedit[x].erase(y);
                    }
                    break;
                }
            }
        }
        tileMap.erase(ID);
    }
}
vector<X9HitTile::RectData*> X9HitTile::hitTest(float x, float y)
{
    const Vec2 loc(x,y);
    vector<RectData*> vs;
    int xIdx = floor(x/tileTap.x);
    int yIdx = floor(y/tileTap.y);
    if (regedit.find(xIdx) != regedit.end() && regedit[xIdx].find(yIdx) != regedit[xIdx].end()) {
        for (int i = 0; i<regedit[xIdx][yIdx].size(); i++) {
            RectData* rectData = tileMap[regedit[xIdx][yIdx][i]];
            if (rectData->rect.containsPoint(loc)) {
                vs.push_back(rectData);
            }
        }
    }
    return vs;
}