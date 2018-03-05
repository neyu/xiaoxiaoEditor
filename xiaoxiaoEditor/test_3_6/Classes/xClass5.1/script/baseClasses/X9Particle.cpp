//
//  X9Particle.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/30.
//
//

#include "X9Particle.h"
#include "X9Node.h"
X9ParticleItem * X9ParticleItem::create(const std::string& filename,float speed, std::function<void()> emptyCallback)
{
    X9ParticleItem* mc = new X9ParticleItem();
    mc->isEmptyCallback = false;
    mc->timeSpace = speed;
    mc->emptyCallback = emptyCallback;
    mc->initWithFile(filename);
    return mc;
}
void X9ParticleItem::update(float dt)
{
    ParticleSystemQuad::update(dt*timeSpace);
    if (_particleCount <= 0 && isEmptyCallback && emptyCallback) {
        emptyCallback();
    }
}

X9ValueObject* base_particle_stop(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"play Error!!!");
    X9Particle* particle = dynamic_cast<X9Particle*>(target);
    particle->_particle->stopSystem();
    return nullptr;
}
void baseSet_particle_isAutoRemoveOnFinish(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isBool(),"set isAutoRemoveOnFinish Error!!!");
    X9Particle* particle = dynamic_cast<X9Particle*>(target);
    particle->_isAutoRemoveOnFinish = value->getBool();
    particle->_particle->isEmptyCallback = value->getBool();
}
X9ValueObject* baseGet_particle_isAutoRemoveOnFinish(X9RunObject* target)
{
    X9Particle* particle = dynamic_cast<X9Particle*>(target);
    return X9ValueObject::createWithBool(particle->_isAutoRemoveOnFinish);
}
void X9Particle::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(particle_,stop);
    
    x9_AddBaseSGet(particle_,isAutoRemoveOnFinish);
//    x9_AddBaseGet(ani_,time);
}
void X9Particle::setConstValues(X9ScriptClassData* classData)
{
//    x9_AddConstValue(String, ENTER_FRAME, ENTER_FRAME);
}
X9Particle* X9Particle::create()
{
    CREATE_NEW_AUTO_OBJ(X9Particle,);
}
X9Particle::X9Particle():X9DisplayObject("X9Particle")
{
    _isAutoRemoveOnFinish = false;
    _particle = nullptr;
    x9_setCtor(Particle);
}
void X9Particle::removed()
{
    _isAutoRemoveOnFinish = false;
    CC_SAFE_RELEASE(_particle);
    X9DisplayObject::removed();
}
void X9Particle::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 1 && vs[0]->isString() && vs[0]->getString() != "","new Animation initObject Error!!!");
    _particle = X9ParticleItem::create(getLibrary()->path->getResourcePath(vs[0]->getString()), getDisplayedTimeSpeed());
    _node = _particle;
}
void X9Particle::timeSpeedChanged()
{
    if (_particle) {
        _particle->timeSpace = getDisplayedTimeSpeed();
    }
}
void X9Particle::addedToStage()
{
    X9DisplayObject::addedToStage();
}
void X9Particle::removedFromStage()
{
    X9DisplayObject::removedFromStage();
}
void X9Particle::update(float delay)
{
}
void X9Particle::emptyCallback()
{
    _parentNode->removeElement(this);
}