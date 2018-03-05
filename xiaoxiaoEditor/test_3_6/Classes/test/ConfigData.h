//
//  ConfigData.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 16/1/6.
//
//

#ifndef ConfigData_hpp
#define ConfigData_hpp

#include "csAppClient.h"
struct ConfigData
{
    struct Card3DEffectData
    {
        float scale;
        float zoomToScale;
        float spaceX;
        float spaceY;
        float angleTime;
        float spaceTime;
        float minSpaceTime;
        float zoomInTime;
        int xCount;
        int yCount;
        vector<string> cards;
        Card3DEffectData(){};
        Card3DEffectData(const rapidjson::Value &value);
    };
    struct SelectRoleData
    {
        struct ItemData
        {
            string name;
            string icon;
            ItemData(){};
            ItemData(const rapidjson::Value &value);
        };
        string background;
        vector<ItemData> items;
        SelectRoleData(){};
        SelectRoleData(const rapidjson::Value &value);
    };
    struct StoryData
    {
        string name;
        string fontName;
        vector<string> texts;
        StoryData(const rapidjson::Value &value);
    };
    struct UIData
    {
        string name;
        Vec2 position;
        UIData(const rapidjson::Value &value);
    };
    Card3DEffectData card3DEffectData;
    vector<SelectRoleData> selectRoleDatas;
    vector<StoryData> storys;
    vector<string> uiFlashMovieClips;
    vector<UIData> xui;
//    SelectRoleData selectRoleData;
    
    ConfigData(){}
    ConfigData(const rapidjson::Value &value);
};
ConfigData* getConfigData();

#endif /* ConfigData_hpp */
