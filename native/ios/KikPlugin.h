//
//  KikAPI.h
//  kikapilib
//
//  Created by Julian Haldenby on 11-03-28.
//  Copyright 2011 Kik Interactive Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef PHONEGAP_FRAMEWORK
    #import <PhoneGap/PGPlugin.h>
    #import <PhoneGap/JSON.h>
    #import <PhoneGap/NSData+Base64.h>
#else
    #import "PGPlugin.h"
    #import "JSON.h"
    #import "NSData+Base64.h"
#endif

#include "lib/KikAPI.h"

#import <Foundation/Foundation.h>


@interface KikPlugin : PGPlugin {
    BOOL _initialized;
}

@property( nonatomic, copy) NSString * callbackID;
/*!
 * We need to listen for a new URL that needs to be opened
 * and then pass it off to the KikAPI
 */
- (void) handleOpenURL:(NSNotification*)notification;
/*!
 * Send a Kik Platform message using this message
 */
- (void) sendMessage:(NSMutableArray *) args withDict:(NSMutableDictionary *) options;

- (void) init:(NSMutableArray*)arguments withDict:(NSMutableDictionary*)options;
/**
 * This will be the call to open the URL
 */
-(void) hasMessage:(NSMutableArray *) args withDict:(NSMutableDictionary *) options;
@end
