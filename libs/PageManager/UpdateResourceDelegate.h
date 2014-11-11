//
//  UpdateResourceDelegate.h
//  HelloLua
//
//  Created by 程小二 on 13-3-25.
//
//

#ifndef HelloLua_UpdateResourceDelegate_h
#define HelloLua_UpdateResourceDelegate_h

typedef enum
{
    NetworkOperationStatusReady = 0,
    NetworkOperationStatusRunning,
    NetworkOperationStatusFinished,
    NetworkOperationStatusCanceled,
    NetworkOperationStatusFailed
} NetworkOperationStatus;


class AssetsManager;

class NetworkOperationDelegate
{
public:
    virtual void operationDidFinish(AssetsManager *operation, const char* strVal) = 0;
    virtual void operationDidFail(AssetsManager *operation) = 0;
    virtual ~NetworkOperationDelegate() {};
};

#endif
