//
//  X9SoundPlayer.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/31.
//
//

#ifndef X9SoundPlayer_hpp
#define X9SoundPlayer_hpp

#include "x9AppClient.h"
class X9Library;
class X9Player;
class X9AniSoundEffectObject
{
public:
    uint ID;
    float beginTime;
public:
    X9AniSoundEffectObject(){};
    X9AniSoundEffectObject(uint ID, float beginTime);
};
class X9SoundPlayer
{
    float _waitTime;
    int _aniIDMAX;
    map<string,float> _effects;
//    map<int,map<string,X9AniSoundEffectObject>> _aniEffects;
public:
    X9Library* library;
    X9Player* player;
public:
    X9SoundPlayer(X9Player* player, X9Library* library, float wait);
    void playEffect(const string& name, bool isAniEffect = false);
    void stopAllEffect();
};

#endif /* X9SoundPlayer_hpp */
