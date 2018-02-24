//
//  X9SoundPlayer.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/31.
//
//

#include "X9SoundPlayer.h"
#include "X9Player.h"

X9AniSoundEffectObject::X9AniSoundEffectObject(uint ID, float beginTime)
{
    this->ID = ID;
    this->beginTime = beginTime;
}
X9SoundPlayer::X9SoundPlayer(X9Player* player, X9Library* library, float wait)
{
    _waitTime = wait;
    _aniIDMAX = 0;
    this->player = player;
    this->library = library;
}
void X9SoundPlayer::playEffect(const string& name, bool isAniEffect)
{
    
    string path = isAniEffect ? library->path->getAnimationPath(name) : library->path->getResourcePath(name);
    SimpleAudioEngine::getInstance()->playEffect(path.c_str());
    /*
    float _timer = library->timer;
    if (_effects.find(name) == _effects.end()) {
        _effects[name] = _timer-_waitTime-1;
    }
    if (_timer - _effects[name] >= _waitTime) {
        _effects[name] = _timer;
        string path = isAniEffect ? library->path->getAnimationPath(name) : library->path->getResourcePath(name);
        SimpleAudioEngine::getInstance()->playEffect(path.c_str());
    }
    for (auto it = _effects.begin(); it != _effects.end(); it++) {
        if (_timer - it->second >= _waitTime) {
            _effects.erase(it);
        }
    }
     */
}
void X9SoundPlayer::stopAllEffect()
{
    _effects.clear();
    SimpleAudioEngine::getInstance()->stopAllEffects();
}