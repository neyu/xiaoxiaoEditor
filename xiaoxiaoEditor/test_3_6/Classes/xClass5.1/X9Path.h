//
//  X9Path.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/9.
//
//

#ifndef X9Path_hpp
#define X9Path_hpp

#include "x9AppClient.h"

class X9Path
{
    vector<string> _frameWorkPaths;
    map<string,string> _scriptMap;
    map<string,string> _resourceMap;
    map<string,string> _animationMap;
    map<string,string> _fontMap;
    map<string,string> _shaderMap;
    map<string,string> _dataMap;
    string _path;
    string getFullPathForName(const string& dir, const string& name);
    
    vector<string> getFilePathAtVec(string filePath);
public:
    string pathName;
    string defaultFrameworksPath;
    string projectFrameworksPath;
    string globalScriptPath;
    string firstSceneScriptPath;
//    string scriptPath;
//    string fontPath;
//    string resourcePath;
//    string animationPath;
    X9Path(const string& path);
    const string& getDataPath(const string& name);
    const string& getScriptPath(const string& name);
    const string& getResourcePath(const string& name);
    const string& getAnimationPath(const string& name);
    const string& getFontPath(const string& name);
    const string& getShaderPath(const string& name);
    const vector<string>& getFrameWorkPaths()const{return _frameWorkPaths;};
    const string& getProjectPath()const {return _path;};
};
#endif /* X9Path_hpp */
