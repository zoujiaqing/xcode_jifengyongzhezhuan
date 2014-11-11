//
//  FileUtiles.h
//  iSeer
//
//  Created by arthurgong on 11-12-21.
//  Copyright 2011Äê __MyCompanyName__. All rights reserved.
//


#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

class FileUtils
{
public:
    FileUtils();
    ~FileUtils();    
    
    static void getFullPath(const char *pzfileName, char* out);
};


#endif // _FILE_UTILS_H_