//
//  X9Particle.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/30.
//
//

#ifndef X9Particle_hpp
#define X9Particle_hpp

#include "X9DisplayObject.h"
class X9ParticleItem : public ParticleSystemQuad
{
public:
    std::function<void()> emptyCallback;
    float timeSpace;
    bool isEmptyCallback;
public:
    static X9ParticleItem * create(const std::string& filename,float speed, std::function<void()> emptyCallback = nullptr);
    virtual void update(float dt) override;
    int getParticleCount(){return _particleCount;};
};
class X9Particle : public X9DisplayObject
{
public:
    bool _isAutoRemoveOnFinish;
    X9ParticleItem* _particle;
    virtual void timeSpeedChanged()override;
    void emptyCallback();
    
    void initObject(const vector<X9ValueObject*>& vs);
public:
    static X9Particle* create();
    X9Particle();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void addedToStage()override;
    virtual void removedFromStage()override;
    virtual void update(float delay)override;
    
public:
};


#endif /* X9Particle_hpp */
