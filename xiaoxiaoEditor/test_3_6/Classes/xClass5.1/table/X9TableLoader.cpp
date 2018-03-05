//
//  X9TableLoader.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/27.
//
//

#include "X9TableLoader.h"

void X9TableItem::TValue::clear(bool isString)
{
    if(isString) {
        delete value.s;
    }
}
X9TableItem::~X9TableItem()
{
    for (int i = 0; i<values.size(); i++) {
        for (int k = 0; k<values[i].size(); k++) {
            values[i][k]->clear(types[k] == Type::STRING);
            delete values[i][k];
        }
    }
    values.clear();
}
X9TableItem* X9TableLoader::load(const string& path)
{
    X9TableItem* item = new X9TableItem();
    string m_path = FileUtils::getInstance()->fullPathForFilename(path);
    CCASSERT(m_path != "","not find path!!!");
    
    string str = String::createWithContentsOfFile(m_path.c_str())->getCString();
    vector<string> tabStrs;
    size_t idx = 0;
    string nKey = "\r\n";
    int nL = 2;
    if (str.find(nKey) == string::npos) {
        nKey = "\r";
        nL = 1;
    }
    CCASSERT(str.find(nKey) != string::npos,"code type path!!!");
    while (true) {
        const size_t n = str.find("\r",idx);
        if (n == string::npos) {
            if (idx > 0) {
                tabStrs.push_back(str.substr(idx));
            }
            break;
        }
        if (idx > 0) {
            tabStrs.push_back(str.substr(idx,n-idx));
        }
        idx = n+nL;
    }
//    log("%s",str.c_str());
    vector<vector<string>> tts(tabStrs.size());
    for (int i = 0; i<tabStrs.size(); i++) {
        if (tabStrs[i] == "") {
            continue;
        }
        size_t idx = 0;
        while (true) {
            const size_t n = tabStrs[i].find("\t",idx);
            if (n == string::npos) {
                tts[i].push_back(tabStrs[i].substr(idx));
                break;
            }
            tts[i].push_back(tabStrs[i].substr(idx,n-idx));
            idx = n+1;
        }
    }
    int count = (int)tts[0].size();
    CCASSERT(tts.size() > 2,"list count ERROR!!!");
    CCASSERT(tts[1].size() == tts[0].size(),"types count ERROR!!!");
    for (int i = 2; i<tts.size(); i++) {
        while (tts[i].size() < tts[0].size()) {
            tts[i].push_back("");
        }
    }
    for (int i = 0; i<count; i++) {
        item->names.push_back(tts[0][i]);
        if (tts[1][i] == "number") {
            item->types.push_back(X9TableItem::NUMBER);
        }else if(tts[1][i] == "string") {
            item->types.push_back(X9TableItem::STRING);
        }else if(tts[1][i] == "bool") {
            item->types.push_back(X9TableItem::BOOL);
        }else{
            CCASSERT(false,"type ERROR!!!");
        }
    }
    for (int i = 2,n = 0; i<tts.size(); i++,n++) {
        if(tts[i][0] == "")
        {
            continue;
        }
        item->values.push_back(vector<X9TableItem::TValue*>());
        for (int k = 0; k<count; k++) {
            item->values.back().push_back(new X9TableItem::TValue());
            switch (item->types[k]) {
                case X9TableItem::NUMBER:
                    if(tts[i][k] == "") tts[i][k] = "0";
                    item->values.back()[k]->value.f = atof(tts[i][k].c_str());
                    break;
                case X9TableItem::BOOL:
                    if(tts[i][k] == "") tts[i][k] = "0";
                    item->values.back()[k]->value.b = (tts[i][k] == "1");
                    break;
                case X9TableItem::STRING:
                    item->values.back()[k]->value.s = new string(tts[i][k]);
                    break;
            }
        }
    }
    return item;
}