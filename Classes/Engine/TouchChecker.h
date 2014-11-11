//
//  TouchChecker.h
//  iSeer
//
//  Created by b a on 12-5-30.
//  Copyright (c) 2012Äê __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_TouchChecker_h
#define iSeer_TouchChecker_h


class ITouchChecker
{ 
public :    
    virtual bool CanTouchNow() = 0;  
    
    virtual bool CanMultiTouchNow() = 0;
};


///
// class TouchChecker, Query can process touch event or Not!
//
class TouchChecker
{    
public :
    TouchChecker() : m_pChecker(0)
    {
       
    }
    
    void SetExternalChecker(ITouchChecker* pkChecker)
    {
        m_pChecker = pkChecker;
    }
    
    bool CanTouchNow()
    {
        if (m_pChecker)
            return m_pChecker->CanTouchNow();
        else
            // default is touch enable
            return true;
    }
    
    bool CanMultiTouchNow()
    {
        if (m_pChecker)
            return m_pChecker->CanMultiTouchNow();
        else
            // default is touch enable
            return true;  
    }
    
protected:
    ITouchChecker* m_pChecker;
};

#endif
