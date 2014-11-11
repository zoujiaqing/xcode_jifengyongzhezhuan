//
//  FileManager.h
//  UpdateResDemo
//
//  Created by Delle  on 13-7-16.
//  Copyright (c) 2013年 Delle . All rights reserved.
//

#ifndef __UpdateResDemo__FileManager__
#define __UpdateResDemo__FileManager__

#include <iostream>
#include "json_res.h"


/** To get file size */
long long  getFileSize(const char *filePath);

/** To get file hash value */
bool getFileHash(const char* filePath, std::string &strHash);

/** To parse json file */
bool parseJsonFile(const char* filePath, Json::Value &jsonValue);

/** To parse json strings */
bool parseJsonStr(const char* jsonStr, Json::Value &jsonValue);

/** To check json file */
bool isExsitInFileList(const std::string &fileName, const Json::Value &jsonValue, int &index);


#endif /* defined(__UpdateResDemo__FileManager__) */
