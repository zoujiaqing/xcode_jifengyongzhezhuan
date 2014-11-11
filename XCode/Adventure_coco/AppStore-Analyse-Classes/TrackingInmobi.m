//
//  TrackingInmobi.m
//  FacebookDemo
//
//  Created by caesarhu on 14-5-8.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import "TrackingInmobi.h"
#import "InMobi.h"
#import "IMBanner.h"
#import "IMBannerDelegate.h"
#import "IMInterstitial.h"
#import "IMInterstitialDelegate.h"
#import "IMError.h"
//#import "IMNetworkExtras.h"
//#import "IMInMobiNetworkExtras.h"
#import "InMobiAnalytics.h"
#import "AppStoreAnalyseConfig.h"
#import <CoreLocation/CoreLocation.h>


static TrackingInmobi *instance = nil;

@implementation TrackingInmobi

- (id)init {
    if (self = [super init]) {
        
    }
    return self;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [super dealloc];
}

+ (TrackingInmobi *)sharedInstance {
    @synchronized(self) {
        if (!instance) {
            instance = [[TrackingInmobi alloc] init];
        }
        return instance;
    }
    return instance;
}

+ (void)purgeSharedInstance {
    @synchronized(self) {
        if (instance) {
            [instance release], instance = nil;
        }
    }
}

- (void)startTracking {
    [InMobi initialize:Tracking_InMobi_Publisher_APP_ID];
    [InMobi setLogLevel:IMLogLevelVerbose];
    [InMobiAnalytics startSessionManually];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterBackGroundNotification:) name:UIApplicationDidEnterBackgroundNotification object:nil];
}

- (void)trackLocation {
    //when using CoreLocation to fetch the coordinate
    CLLocationManager *mgr = [[[CLLocationManager alloc] init] autorelease];
    CLLocation *loc = mgr.location;
    [InMobi setLocationWithLatitude:loc.coordinate.latitude longitude:loc.coordinate.longitude accuracy:loc.horizontalAccuracy];
    [mgr startUpdatingLocation];
}

- (void)trackEvent:(NSString *)event withParamters:(NSDictionary *)params {
    if (params) {
        [InMobiAnalytics tagEvent:event withParams:params];
    }else {
        [InMobiAnalytics tagEvent:event];
    }
}

- (void)beginTrackSectionEvent:(NSString *)event withParamters:(NSDictionary *)params {
    if (params) {
        [InMobiAnalytics beginSection:event withParams:params];
    }else {
        [InMobiAnalytics beginSection:event];
    }
}

- (void)endTrackSectionEvent:(NSString *)event withParamters:(NSDictionary *)params {
    if (params) {
        [InMobiAnalytics endSection:event withParams:params];
    }else {
        [InMobiAnalytics endSection:event];
    }
}

- (void)trackTransaction:(id)transacton {
    [InMobiAnalytics tagTransactionManually:transacton];
}

- (void)showBannerWithRect:(CGRect)rect size:(IM_BannerSize)size inView:(UIView *)view refreshInterval:(int)sec {
    IMBanner *banner = [[[IMBanner alloc] initWithFrame:rect appId:Tracking_InMobi_Property_ID adSize:size] autorelease];
    if (sec < 0) {
        banner.refreshInterval = REFRESH_INTERVAL_OFF;
    }else if (sec >0) {
        banner.refreshInterval = sec;
    }
    [view addSubview:banner];
    [banner loadBanner];
}

- (void)showInterstitial {
    IMInterstitial *adInterstitial = [[[IMInterstitial alloc] initWithAppId:Tracking_InMobi_Property_ID] autorelease];
    [adInterstitial loadInterstitial];
}



- (void)didEnterBackGroundNotification:(NSNotification *)notification {
    [InMobiAnalytics endSessionManually];
}
































@end
