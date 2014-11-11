//
//  FlurryMan.m
//  FacebookDemo
//
//  Created by caesarhu on 14-5-4.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import "FlurryMan.h"
#import "Flurry.h"
#import <CoreLocation/CoreLocation.h>


#define FLURRY_CHANNEL_INFO_AHERO  @"FLURRY_CHANNEL_INFO_AHERO"

@implementation FlurryMan


+ (void)initFlurryWithChannel:(NSString *)channel isTrackLocation:(BOOL)isTrackLocation isNeedCrashReport:(BOOL)isNeedCrashReport isAppStore:(BOOL)isAppStore {
    [Flurry setCrashReportingEnabled:isNeedCrashReport];
    if (isAppStore) {
        [Flurry startSession:FLURRY_APPSTORE_API_KEY];
    }else {
        [Flurry startSession:FLURRY_JAILBREAK_API_KEY];
    }
    
    if (nil == [[NSUserDefaults standardUserDefaults] objectForKey:FLURRY_CHANNEL_INFO_AHERO]) {
        [[NSUserDefaults standardUserDefaults] setObject:channel forKey:FLURRY_CHANNEL_INFO_AHERO];
        [Flurry logEvent:@"Channel info" withParameters:[NSDictionary dictionaryWithObjectsAndKeys:channel, @"Game Channel ID", nil]];
    }
    if (isTrackLocation) {
        CLLocationManager *locationManager = [[[CLLocationManager alloc] init] autorelease];
        [locationManager startUpdatingLocation];
        CLLocation *location = locationManager.location;
        [Flurry setLatitude:location.coordinate.latitude longitude:location.coordinate.longitude horizontalAccuracy:location.horizontalAccuracy verticalAccuracy:location.verticalAccuracy];
    }
}

+ (void)trackEvent:(NSString *)eventName withParameters:(NSDictionary *)dic {
    if (dic) {
        [Flurry logEvent:eventName withParameters:dic];
    }else {
        [Flurry logEvent:eventName];
    }
    
}

@end
