//
//  EventID.h
//  iSeer
//
//  Created by b a on 12-5-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_EventID_h
#define iSeer_EventID_h

//-------------定义Notification Center Message ID------------
//                           全局范围-----------
const int NCM_BASE_MESSAGE   = 0x01000000;
const int NCM_GAMEMANGER_CONSTRUST   = NCM_BASE_MESSAGE + 1;


//-------------定义Event ID------------全局范围-----------
typedef unsigned int  EVENTID;

// Net Package Event id definition
const EVENTID       NET_PACKAGE_BASE = 0X00000001;

const EVENTID       NET_PACKAGE_RECEIVE = NET_PACKAGE_BASE + 1;  // receive net package
const EVENTID       NET_PACKAGE_SEND    = NET_PACKAGE_BASE + 2;  // send net package


// Engine Event id definition
const EVENTID      ENGINE_EVENT_BASE = 0X00001000;
const EVENTID      EVENT_TRIGGER       = ENGINE_EVENT_BASE + 1;
const EVENTID      EVENT_SPRITE_STAR   = ENGINE_EVENT_BASE + 2;
const EVENTID      EVENT_SPRITE_TRANSPOINT  = ENGINE_EVENT_BASE + 3;
const EVENTID      EVENT_SPRITE_ELF    = ENGINE_EVENT_BASE + 4;
const EVENTID      EVENT_TOUCH_TRAIN   = ENGINE_EVENT_BASE + 5;
const EVENTID      EVENT_TOUCH_STORAGE = ENGINE_EVENT_BASE + 6;


// Gamelogic Event id definition
const EVENTID      GAME_LOGIC_BASE  = 0X00002000;
const EVENTID      LOGIC_MINE_COMPLETE = GAME_LOGIC_BASE + 1;


// to do .... Add more here!

#endif
