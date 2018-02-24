//
//  X9Array.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/28.
//
//

#include "X9Array.h"
#include "../X9RunScript.h"

/**
 * @function:push
 * @param:* value
 * @return:number length
 *
 */
X9ValueObject* base_arr_push(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(!values.empty(),"push Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    for (int i = 0; i<values.size(); i++) {
        values[i]->use();
        arr->children.push_back(values[i]);
    }
    return X9ValueObject::createWithNumber((float)arr->children.size());
}
/**
 * @function:pop
 * @return:* value
 *
 */
X9ValueObject* base_arr_pop(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"pop Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    X9ASSERT(!arr->children.empty(),"pop Error!!!");
    X9ValueObject* v = arr->children.back()->clone();
    arr->children.erase(arr->children.end()-1);
    return v;
}
/**
 * @function:unshift
 * @param:* value
 * @return:number length
 *
 */
X9ValueObject* base_arr_unshift(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(!values.empty(),"unshift Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    for (int i = 0; i<values.size(); i++) {
        values[i]->use();
        arr->children.insert(arr->children.begin(), values[i]);
    }
    return X9ValueObject::createWithNumber((float)arr->children.size());
}
/**
 * @function:shift
 * @return:* value
 *
 */
X9ValueObject* base_arr_shift(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"shift Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    X9ASSERT(!arr->children.empty(),"shift Error!!!");
    X9ValueObject* v = arr->children.front()->clone();
    arr->children.erase(arr->children.begin());
    return v;
}
/**
 * @function:at
 * @param:number index
 * @return:* value
 *
 */
X9ValueObject* base_arr_get(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"at Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    int idx = values[0]->getNumber();
    X9ASSERT(idx >= 0 && idx < arr->children.size(),"at Error!!!");
    return arr->children[idx]->clone();
}
/**
 * @function:set
 * @param:number index
 * @param:* value
 * @return:null
 *
 */
X9ValueObject* base_arr_set(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isNumber(),"set Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    int idx = values[0]->getNumber();
    X9ASSERT(idx >= 0 && idx <= arr->children.size(),"set Error!!!");
    values[1]->use();
    if (idx < arr->children.size()) {
        arr->children[idx]->setValueObject(values[1]);
    }else{
        arr->children.push_back(values[1]);
    }
    return nullptr;
}
/**
 * @function:empty
 * @return:bool
 *
 */
X9ValueObject* base_arr_empty(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"empty Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    return X9ValueObject::createWithBool(arr->children.empty());
}
///**
// * @function:size
// * @return:number length
// *
// */
//X9ValueObject* base_arr_size(X9RunObject* target,const vector<X9ValueObject*>& values)
//{
//    X9ASSERT(values.empty(),"size Error!!!");
//    X9Array* arr = dynamic_cast<X9Array*>(target);
//    return X9ValueObject::createWithNumber(arr->children.size());
//}
/**
 * @function:concat
 * @param:Array value
 * @return:Array new array
 *
 */
X9ValueObject* base_arr_concat(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9Array* arr = dynamic_cast<X9Array*>(target);
    vector<X9ValueObject*> oldVs;
    for (int i = 0; i<arr->children.size(); i++) {
        oldVs.push_back(arr->children[i]->clone());
    }
    X9Array* newArr = dynamic_cast<X9Array*>(target->getLibrary()->createObject("Array", oldVs));
    for (int i = 0; i<values.size(); i++) {
        values[i]->use();
        newArr->children.push_back(values[i]);
    }
    return X9ValueObject::createWithObject(newArr);
}
/**
 * @function:indexOf
 * @param:* value
 * @param:number index
 * @return:number index
 *
 */
X9ValueObject* base_arr_indexOf(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(!values.empty() && values.size() <= 2,"indexOf Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    if (arr->children.empty()) {
        return X9ValueObject::createWithNumber(-1);
    }
    int stIdx = 0;
    if (values.size() == 2) {
        X9ASSERT(values[1]->isNumber(),"indexOf Error!!!");
        stIdx = MAX(0,values[1]->getNumber());
    }
    if (stIdx >= arr->children.size()) {
        return X9ValueObject::createWithNumber(-1);
    }
    for (int i = stIdx; i<arr->children.size(); i++) {
        if ((*arr->children[i] == values[0])->getBool()) {
            return X9ValueObject::createWithNumber(i);
        }
    }
    return X9ValueObject::createWithNumber(-1);
}
/**
 * @function:lastIndexOf
 * @param:* value
 * @param:number index
 * @return:number index
 *
 */
X9ValueObject* base_arr_lastIndexOf(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(!values.empty() && values.size() <= 2,"lastIndexOf Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    if (arr->children.empty()) {
        return X9ValueObject::createWithNumber(-1);
    }
    int stIdx = arr->children.size()-1;
    if (values.size() == 2) {
        X9ASSERT(values[1]->isNumber(),"lastIndexOf Error!!!");
        stIdx = MIN(stIdx,values[1]->getNumber());
    }
    if (stIdx < 0) {
        return X9ValueObject::createWithNumber(-1);
    }
    for (int i = stIdx; i>=0; i--) {
        if ((*arr->children[i] == values[0])->getBool()) {
            return X9ValueObject::createWithNumber(i);
        }
    }
    return X9ValueObject::createWithNumber(-1);
}
/**
 * @function:join
 * @param:string str
 * @return:string
 *
 */
X9ValueObject* base_arr_join(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"join Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    const string& str = values[0]->getString();
    std::ostringstream oss;
    for (int i = 0; i<arr->children.size(); i++) {
        if (i > 0) {
            oss<<str;
        }
        oss<<arr->children[i]->getString();
    }
    return X9ValueObject::createWithString(oss.str());
}
/**
 * @function:sort
 * @param:Function func
 * @return:null
 *
 */
X9ValueObject* base_arr_sort(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isFunction(),"sort Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    X9FunctionValueObject* func = values[0]->getFunction();
    vector<X9ValueObject*> vs(2);
    for (int i = 0; i<arr->children.size()-1; i++) {
        for (int k = i+1; k<arr->children.size(); k++) {
            vs[0] = arr->children[i]->clone();
            vs[1] = arr->children[k]->clone();
            if(func->runFunction(vs)->getBool())
            {
                X9ValueObject* a = arr->children[i];
                arr->children[i] = arr->children[k];
                arr->children[k] = a;
            }
        }
    }
    return X9ValueObject::create();
}
/**
 * @function:reverse
 * @return:null
 *
 */
X9ValueObject* base_arr_reverse(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"reverse Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    vector<X9ValueObject*> vs = arr->children;
    for (int i = 0; i<arr->children.size(); i++) {
        arr->children[i] = vs[arr->children.size()-1-i];
    }
    return nullptr;
}
/**
 * @function:slice
 * @param:number startIndex
 * @param:number endIndex
 * @return:Array
 *
 */
X9ValueObject* base_arr_slice(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 || values.size() == 2,"slice Error!!!");
    X9ASSERT(values[0]->isNumber(),"slice Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    int stIdx = MAX(0,values[0]->getNumber());
    int edIdx = arr->children.size();
    if (values.size() == 2) {
        X9ASSERT(values[1]->isNumber(),"slice Error!!!");
        edIdx = MAX(stIdx+1,MIN(edIdx,values[1]->isNumber()));
    }
    vector<X9ValueObject*> newVs;
    for (int i = stIdx;i<edIdx; i++) {
        newVs.push_back(arr->children[i]->clone());
    }
    return target->getLibrary()->createValueObject("Array", newVs);
}
/**
 * @function:splice (splice(0),splice(0,1),splice(0,1,...))
 * @param:number startIndex
 * @param:number removeCount
 * @param:... add values
 * @return:Array
 *
 */
X9ValueObject* base_arr_splice(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(!values.empty(),"splice Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    if (arr->children.empty()) {
        return target->getLibrary()->createValueObject("Array");
    }
    int stIdx = 0;
    int edIdx = arr->children.size();
    vector<X9ValueObject*> vs;
    X9ASSERT(values[0]->isNumber(),"splice Error!!!");
    stIdx = MAX(0,values[0]->getNumber());
    if (stIdx >= arr->children.size()) {
        return target->getLibrary()->createValueObject("Array");
    }
    if (values.size() >= 2) {
        X9ASSERT(values[1]->isNumber(),"splice Error!!!");
        edIdx = MIN(arr->children.size(),MAX(stIdx,values[1]->getNumber()+stIdx));
    }
    if (edIdx != stIdx) {
        for (int i = stIdx; i<edIdx; i++) {
            vs.push_back(arr->children[i]->clone());
            arr->children[i]->nonuse();
        }
        arr->children.erase(arr->children.begin()+stIdx, arr->children.begin()+edIdx);
    }
    if (values.size() > 2) {
        for (int i = 2,k = stIdx; i<values.size(); i++,k++) {
            values[i]->use();
            arr->children.insert(arr->children.begin()+stIdx, values[i]);
        }
    }
    return target->getLibrary()->createValueObject("Array", vs);
}
X9ValueObject* base_arr_clear(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"splice Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    for (int i = 0; i<arr->children.size(); i++) {
        arr->children[i]->nonuse();
    }
    arr->children.clear();
    return X9ValueObject::create();
}
X9ValueObject* base_arr_insert(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isNumber(),"insert Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    int idx = MAX(0,MIN(arr->children.size(),(int)values[0]->getNumber()));
    arr->children.insert(arr->children.begin()+idx, values[1]);
    values[1]->use();
    return X9ValueObject::create();
}
X9ValueObject* base_arr_erase(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"erase Error!!!");
    X9Array* arr = dynamic_cast<X9Array*>(target);
    int idx = MAX(0,MIN(arr->children.size(),(int)values[0]->getNumber()));
    arr->children[idx]->nonuse();
    arr->children.erase(arr->children.begin()+idx);
    return X9ValueObject::create();
}

X9ValueObject* baseGet_arr_length(X9RunObject* target)
{
    X9Array* arr = dynamic_cast<X9Array*>(target);
    return X9ValueObject::createWithNumber(arr->children.size());
}
X9ValueObject* baseGet_arr_front(X9RunObject* target)
{
    X9Array* arr = dynamic_cast<X9Array*>(target);
    if (arr->children.empty()) {
        return X9ValueObject::create();
    }
    return arr->children.front()->clone();
}
X9ValueObject* baseGet_arr_back(X9RunObject* target)
{
    X9Array* arr = dynamic_cast<X9Array*>(target);
    if (arr->children.empty()) {
        return X9ValueObject::create();
    }
    return arr->children.back()->clone();
}

void X9Array::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(arr_,push);
    x9_AddBaseFunc(arr_,pop);
    x9_AddBaseFunc(arr_,unshift);
    x9_AddBaseFunc(arr_,shift);
    x9_AddBaseFunc(arr_,get);
    x9_AddBaseFunc(arr_,set);
    x9_AddBaseFunc(arr_,empty);
//    x9_AddBaseFunc(arr_,size);
    
    x9_AddBaseFunc(arr_,concat);
    x9_AddBaseFunc(arr_,indexOf);
    x9_AddBaseFunc(arr_,lastIndexOf);
    x9_AddBaseFunc(arr_,join);
    x9_AddBaseFunc(arr_,sort);
    x9_AddBaseFunc(arr_,reverse);
    x9_AddBaseFunc(arr_,slice);
    x9_AddBaseFunc(arr_,splice);
    x9_AddBaseFunc(arr_,clear);
    x9_AddBaseFunc(arr_,insert);
    x9_AddBaseFunc(arr_,erase);
    
    x9_AddBaseGet(arr_, length);
    x9_AddBaseGet(arr_, front);
    x9_AddBaseGet(arr_, back);
}
void X9Array::setConstValues(X9ScriptClassData* classData)
{
}

X9Array* X9Array::newArray(X9Library* library)
{
    return dynamic_cast<X9Array*>(library->createObject("Array"));
}
X9Array* X9Array::create()
{
    CREATE_NEW_AUTO_OBJ(X9Array,);
}
X9Array::X9Array():X9Object("X9Array")
{
    x9_setCtor(Array);
}

void X9Array::removed()
{
    for (int i = 0; i<children.size(); i++) {
        children[i]->nonuse();
    }
    children.clear();
    X9Object::removed();
}
/**
 * @class:Array
 * @param:string name
 * @return:bool
 *
 */
void X9Array::initObject(const vector<X9ValueObject*>& vs)
{
    for (int i = 0; i<vs.size(); i++) {
        vs[i]->use();
        children.push_back(vs[i]);
    }
}
void X9Array::push(X9ValueObject* v)
{
    v->use();
    children.push_back(v);
}
void X9Array::unshift(X9ValueObject* v)
{
    v->use();
    children.insert(children.begin(), v);
}
void X9Array::pushNumber(float value)
{
    push(X9ValueObject::createWithNumber(value));
}
void X9Array::pushBool(bool value)
{
    push(X9ValueObject::createWithBool(value));
}
void X9Array::pushString(const string& value)
{
    push(X9ValueObject::createWithString(value));
}
void X9Array::pushObject(X9Object* value)
{
    push(X9ValueObject::createWithObject(value));
}
void X9Array::unshiftNumber(float value)
{
    unshift(X9ValueObject::createWithNumber(value));
}
void X9Array::unshiftBool(bool value)
{
    unshift(X9ValueObject::createWithBool(value));
}
void X9Array::unshiftString(const string& value)
{
    unshift(X9ValueObject::createWithString(value));
}
void X9Array::unshiftObject(X9Object* value)
{
    unshift(X9ValueObject::createWithObject(value));
}
X9ValueObject* X9Array::pop()
{
    X9ValueObject* v = children.back();
    v->nonuse();
    children.erase(children.end()-1);
    return v;
}
X9ValueObject* X9Array::shift()
{
    X9ValueObject* v = children.front();
    v->nonuse();
    children.erase(children.begin());
    return v;
}
string X9Array::getJsonString()
{
    string str = "[";
    for(int i = 0;i<children.size();i++) {
        if (i > 0) {
            str += ",";
        }
        str += children[i]->getJsonString();
    }
    return str+"]";
}
string X9Array::toString()
{
    std::ostringstream oss;
    oss<<"[";
    for (int i = 0; i<children.size(); i++) {
        if (i > 0) {
            oss<<",";
        }
        oss<<children[i]->getString();
    }
    oss<<"]";
    return oss.str();
}