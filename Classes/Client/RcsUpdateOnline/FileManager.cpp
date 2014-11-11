//
//  FileManager.cpp
//  UpdateResDemo
//
//  Created by Delle  on 13-7-16.
//  Copyright (c) 2013年 Delle . All rights reserved.
//

#include "FileManager.h"
#include <sys/stat.h>
#include "openssl/sha.h"
#include "openssl/bio.h"

#define BUFF_SIZE 1024
#define FILE_SIZE 1024*1024


long long getFileSize(const char *filePath)
{
    struct stat info;
    stat(filePath, &info);
    long long  size = info.st_size;
    return size;
}

bool getFileHash( const char *filePath, std::string &strHash)
{
    FILE *f = fopen(filePath, "rb");
    if (f == NULL) {
        return false;
    }
    
    void *data = malloc(BUFF_SIZE);
    if (data == NULL) {
        return false;
    }
    
    SHA_CTX c;
    SHA1_Init(&c);

    unsigned char *md      = (unsigned char *) malloc(512);
    unsigned long fileSize =(unsigned long) getFileSize(filePath);
    
    fseek(f, 0, 0);
    while (1) {
        size_t count =  fread(data, BUFF_SIZE, 1, f);
        if (count < 1) {
            if (fileSize > BUFF_SIZE) { /** Read file error */
                return false;
            }
            SHA1_Update(&c, data,fileSize);
            break;
        }
        else
        {
            SHA1_Update(&c, data,BUFF_SIZE);
        }
        fileSize -= BUFF_SIZE;
        memset(data, 0, BUFF_SIZE);
    }
    
    SHA1_Final(md, &c);
    
    //  To convert unsigned char to char.
    char b[41];
    char *ptr = b;
    for (int i=0; i<20; i++) {
        sprintf(ptr, "%02x", md[i]);
        ptr +=2;
    }
    b[40] = '\0';
    std::string str = b;
    strHash = str;
    free(md);
    return true;
}

bool parseJsonFile(const char* filePath, Json::Value &jsonValue)
{
    FILE *f = fopen(filePath, "rb");
    if (f == NULL) {
        return false;
    }
    
    void *data = malloc(FILE_SIZE);
    if (data == NULL) {
        return false;
    }
    fread(data, 1024, 20, f);
    char *tmp =(char *) data;
    std::string str(tmp);
    
    Json::Reader jsonReader;
    bool ret = jsonReader.parse(str, jsonValue);
    if (!ret) {
        return false;
    }
    return true;
}

bool parseJsonStr(const char* jsonStr, Json::Value &jsonValue)
{
    std::string str(jsonStr);
    Json::Reader jsonReader;
    bool ret = jsonReader.parse(str, jsonValue);
    return ret;
}

bool isExsitInFileList(const std::string &fileName, const Json::Value &jsonValue, int &index)
{
    const Json::Value arrayObj = jsonValue["files"];
    for (int i=0; i<arrayObj.size(); i++) {
        std::string str = arrayObj[i]["hash"].asString();
        
        if (str == fileName) {
            index = i;
            return true;
        }
    }
    
    index = -1;
    
    return false;
}

