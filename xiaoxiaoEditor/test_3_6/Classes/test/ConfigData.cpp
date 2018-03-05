//
//  ConfigData.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 16/1/6.
//
//

#include "ConfigData.h"

#define AddConfigChild(__N__,__C__) __N__ = __C__(value[#__N__]);

ConfigData __configData__;
bool __isFirstLoad__ = true;
ConfigData::Card3DEffectData::Card3DEffectData(const rapidjson::Value &value)
{
    GetJsonValue(value, scale, Double, 0.45);
    GetJsonValue(value, zoomToScale, Double, 1.25);
    GetJsonValue(value, spaceX, Double, 180);
    GetJsonValue(value, spaceY, Double, 260);
    GetJsonValue(value, angleTime, Double, 0.75);
    GetJsonValue(value, spaceTime, Double, 0.56);
    GetJsonValue(value, minSpaceTime, Double, 0.1);
    GetJsonValue(value, zoomInTime, Double, 0.65);
    GetJsonValue(value, xCount, Int, 5);
    GetJsonValue(value, yCount, Int, 2);
    if (value.HasMember("cards")) {
        for (int i = 0; i<value["cards"].Size(); i++) {
            cards.push_back(value["cards"][i].GetString());
        }
    }
}
ConfigData::SelectRoleData::SelectRoleData(const rapidjson::Value &value)
{
    GetJsonValueO(value, background, String);
    for (int i = 0; i<value["items"].Size(); i++) {
        items.push_back(ItemData(value["items"][i]));
    }
}
ConfigData::SelectRoleData::ItemData::ItemData(const rapidjson::Value &value)
{
    GetJsonValueO(value, name, String);
    GetJsonValueO(value, icon, String);
}
ConfigData::StoryData::StoryData(const rapidjson::Value &value)
{
    GetJsonValueO(value, name, String);
    GetJsonValueO(value, fontName, String);
    for (int i = 0; i<value["texts"].Size(); i++) {
        texts.push_back(value["texts"][i].GetString());
    }
}
ConfigData::UIData::UIData(const rapidjson::Value &value)
{
    GetJsonValueO(value, name, String);
    float x;
    float y;
    GetJsonValueO(value, x, Double);
    GetJsonValueO(value, y, Double);
    position.x = x;
    position.y = y;
}
ConfigData::ConfigData(const rapidjson::Value &value)
{
    AddConfigChild(card3DEffectData,Card3DEffectData);
    for (int i = 0; i<value["selectRoleDatas"].Size(); i++) {
        selectRoleDatas.push_back(SelectRoleData(value["selectRoleDatas"][i]));
    }
    for (int i = 0; i<value["storys"].Size(); i++) {
        storys.push_back(StoryData(value["storys"][i]));
    }
    for (int i = 0; i<value["uiFlashMovieClips"].Size(); i++) {
        uiFlashMovieClips.push_back(value["uiFlashMovieClips"][i].GetString());
    }
    for (int i = 0; i<value["xui"].Size(); i++) {
        xui.push_back(UIData(value["xui"][i]));
    }
//    AddConfigChild(selectRoleData,SelectRoleData);
}
ConfigData* getConfigData()
{
    if (__isFirstLoad__) {
        __isFirstLoad__ = false;
        getJsonDocument(_dot,"userDefault.json");
        __configData__ = ConfigData(_dot);
    }
    return &__configData__;
}