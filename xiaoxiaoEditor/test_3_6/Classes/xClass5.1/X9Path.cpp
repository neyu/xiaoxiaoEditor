//
//  X9Path.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/9.
//
//

#include "X9Path.h"
#include "XJsonMacrocoding.h"

X9Path::X9Path(const string& path):pathName(path)
{
    
    _path = "x9project/"+path+"/";
    projectFrameworksPath = _path+"frameworks";
    defaultFrameworksPath = "x9frameworks";
    vector<string> fpath1s = getFilePathAtVec(projectFrameworksPath);
    vector<string> fpath2s = getFilePathAtVec(defaultFrameworksPath);
    for (int i = 0; i<fpath1s.size(); i++) {
        _frameWorkPaths.push_back(projectFrameworksPath+"/"+fpath1s[i]);
    }
    for (int i = 0; i<fpath2s.size(); i++) {
        _frameWorkPaths.push_back(defaultFrameworksPath+"/"+fpath2s[i]);
    }
    globalScriptPath = _path+"scripts/Global.x9as";
    firstSceneScriptPath = _path+"scripts/Main.x9as";
    if(FileUtils::getInstance()->fullPathForFilename(globalScriptPath) == "")
    {
        globalScriptPath = "";
    }
    if(FileUtils::getInstance()->fullPathForFilename(firstSceneScriptPath) == "")
    {
        firstSceneScriptPath = "";
    }
}
string X9Path::getFullPathForName(const string& dir, const string& name)
{
    string path = _path+dir+name;
    string p = FileUtils::getInstance()->fullPathForFilename(path);
    for (int i = 0; i<_frameWorkPaths.size() && p == ""; i++) {
        path = _frameWorkPaths[i]+"/"+dir+name;
        p = FileUtils::getInstance()->fullPathForFilename(path);
    }
    return p;
}
vector<string> X9Path::getFilePathAtVec(string filePath)
{
    vector<std::string> path_vec;
    filePath = FileUtils::getInstance()->fullPathForFilename(filePath);
    if (filePath == "") {
        return path_vec;
    }
#ifdef WIN32 

#else
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int i=0;
    
    if((dp=opendir(filePath.c_str()))==NULL)
    {
        return path_vec;
    }
    chdir(filePath.c_str());
    
    while((entry=readdir(dp))!=NULL&&i<255)
    {
        stat(entry->d_name,&statbuf);
        //        log("name:%s",entry->d_name);
        if(!S_ISDIR(statbuf.st_mode))
            continue;
        string p = StringUtils::format("%s",entry->d_name);
        if (p == "." || p == "..") {
            continue;
        }
        path_vec.push_back(p);
    }
#endif
    return path_vec;
}
const string& X9Path::getDataPath(const string& name)
{
    if (_dataMap.find(name) == _dataMap.end()) {
        string path = getFullPathForName("datas/",name);
        std::ostringstream oss;
        oss<<"No file found:";
        oss<<name;
        CCASSERT(path != "",oss.str().c_str());
        _dataMap[name] = path;
    }
    return _dataMap[name];
}
const string& X9Path::getScriptPath(const string& name)
{
    if (_scriptMap.find(name) == _scriptMap.end()) {
        string path = getFullPathForName("scripts/",name+".x9as");
        std::ostringstream oss;
        oss<<"No file found:";
        oss<<name;
        CCASSERT(path != "",oss.str().c_str());
        _scriptMap[name] = path;
    }
    return _scriptMap[name];
}
const string& X9Path::getResourcePath(const string& name)
{
    if (_resourceMap.find(name) == _resourceMap.end()) {
        string path = getFullPathForName("flResources/",name);
        if (path == "") {
            path = getFullPathForName("resources/",name);
        }
        std::ostringstream oss;
        oss<<"No file found:";
        oss<<name;
        CCASSERT(path != "",oss.str().c_str());
        _resourceMap[name] = path;
    }
    return _resourceMap[name];
}
const string& X9Path::getAnimationPath(const string& name)
{
    if (_animationMap.find(name) == _animationMap.end()) {
        string path = getFullPathForName("flResources/animations/",name);
        std::ostringstream oss;
        oss<<"No file found:";
        oss<<name;
        CCASSERT(path != "",oss.str().c_str());
        _animationMap[name] = path;
    }
    return _animationMap[name];
}
const string& X9Path::getFontPath(const string& name)
{
    if (_fontMap.find(name) == _fontMap.end()) {
        string path = getFullPathForName("fonts/",name);
        std::ostringstream oss;
        oss<<"No file found:";
        oss<<name;
        CCASSERT(path != "",oss.str().c_str());
        _fontMap[name] = path;
    }
    return _fontMap[name];
}
const string& X9Path::getShaderPath(const string& name)
{
    if (_shaderMap.find(name) == _shaderMap.end()) {
        string path = getFullPathForName("shaders/",name);
        std::ostringstream oss;
        oss<<"No file found:";
        oss<<name;
        CCASSERT(path != "",oss.str().c_str());
        _shaderMap[name] = path;
    }
    return _shaderMap[name];
}
