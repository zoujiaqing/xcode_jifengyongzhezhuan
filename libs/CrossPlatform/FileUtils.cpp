//
//  FileUtiles.cpp
//  iSeer
//
//  Created by arthurgong on 11-12-21.
//  Copyright 2011Äê __MyCompanyName__. All rights reserved.
//
#include "CCFileUtils.h"
#include "FileUtils.h"

#include <iostream>
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

FileUtils::FileUtils()
{
    
}

FileUtils::~FileUtils()
{
    
}

void FileUtils::getFullPath(const char *pzfileName, char* out)
{
#ifdef __APPLE__  
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    strcat(path, pzfileName); 
    std::cout << "Current Path: " << path << std::endl;
    
    strcat(out, path);
#endif

}