/**
 * Copyright (C) 2011 by Kik Interactive
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
//
//  KikPlugin.m
//  KikAPIPhoneGap
//
//  Created by Julian Haldenby on 11-09-26.
//  Copyright 2011 Kik Interactive Inc. All rights reserved.
//

#import "KikPlugin.h"

@interface KikPlugin (Private)

/**
 * This will serializa an APIMessage into an NSDictionary
 */
+ (NSDictionary *) dictionaryFromAPIMessage:(KikAPIMessage *) message;

/**
 * This will create a KikAPIPlugin from dictionary
 */
+ (KikAPIMessage *) KikAPIMessageFromDict:(NSDictionary *) dictionary;
/**
 * Here we will verify if a string should be valid
 */
+ (BOOL) isNilOrEmpty:(NSString *) string;
/**
 * Register as a kik plugin
 */
 
-(void) registerAsKikHandler:(NSString *) homepage addAppButton:(BOOL) button;
@end

@implementation KikPlugin

@synthesize callbackID;

- (void) init:(NSMutableArray*)arguments withDict:(NSMutableDictionary*)options
{
    if([arguments count] == 3)
    {
        self.callbackID = [arguments pop];
       
            NSString * url = [arguments pop];
            NSNumber * reg = [arguments pop];
            
            BOOL button = [reg compare:[NSNumber numberWithBool:YES]];
            
            
            [self registerAsKikHandler:url addAppButton:button];
            
            PluginResult * pluginResult = [PluginResult resultWithStatus:PGCommandStatus_OK];
            [self writeJavascript: [pluginResult toSuccessCallbackString:self.callbackID] ];
            //NSString * function = @"KikAPIClient.message_ready(
        } else {
            
            // Oops, Something has shit in the bed
            PluginResult * pluginResult = [PluginResult resultWithStatus:PGCommandStatus_ERROR];
            [self writeJavascript: [pluginResult toErrorCallbackString:self.callbackID]];
            return;
        }
        
}

-(void) registerAsKikHandler:(NSString *) homepage addAppButton:(BOOL) button
{
    _initialized = YES;
    NSBundle * bundle = [NSBundle mainBundle];
	[KikAPIClient registerAsKikPluginWithAppID:[bundle bundleIdentifier] withHomepageURI:homepage addAppButton:button];

}

- (void) handleOpenURL:(NSNotification*)notification
{
    // Retrieve the url from the notification
    NSURL * kik_api_url = [notification object];
    
    if ([kik_api_url isKindOfClass:[NSURL class]] ) {
        KikAPIData * data = [KikAPIClient handleOpenURL:kik_api_url];
        // This will format our current directory, as well as create
        // an inner dictionary for the API Plugin
        NSDictionary * main_dictionary = [NSDictionary dictionaryWithObjectsAndKeys:data.conversationID,@"conversationID",
                                          [NSNumber numberWithInt:data.type],@"type", // Are we invoked from the media tray or other
                                     [NSNumber numberWithBool:data.isOutgoing],@"isOutgoing", 
                                     data.myName, @"myName",
                                     data.senderName, @"senderName",
                                     [KikPlugin dictionaryFromAPIMessage:data.message],@"message",
                                     nil];
        
        PluginResult * p_result = [PluginResult resultWithStatus:PGCommandStatus_OK messageAsDictionary:main_dictionary];
        NSString * callback = [p_result toJSONString];
       
        //
        // Now we callback to the Kik API Javascript library and we begin a timeout so that we do not block the UI
        //
        NSString * jsonRepresenation = [NSString stringWithFormat:@"setTimeout(function() { KikAPIClient.message_ready( '%@' ); }, 0 );",callback];
        NSLog(@"%@",jsonRepresenation);
        [super writeJavascript:jsonRepresenation];
        
    } else {
        PluginResult * p_result = [PluginResult resultWithStatus:PGCommandStatus_ERROR];
        NSString * callback = [p_result toErrorCallbackString:self.callbackID];
        
        //
        // Now we callback with the javascript:
        //
        [super writeJavascript:callback];
    }
    
}



+ (NSDictionary *) dictionaryFromAPIMessage:(KikAPIMessage *) message
{
    NSMutableDictionary * dictionary = [[NSMutableDictionary alloc] init];
    
    
        [dictionary setValue:message.title forKey:@"title"];
        [dictionary setValue:message.description forKey:@"description"];
        [dictionary setValue:message.genericURIs forKey:@"genericURIs"];
        [dictionary setValue:message.androidURIs forKey:@"androidURIs"];
        [dictionary setValue:message.iphoneURIs forKey:@"iphoneURIs"];
        [dictionary setValue:message.extras forKey:@"extras"];
        [dictionary setValue:message.fileUrl forKey:@"fileUrl"]; 
        [dictionary setValue:[NSNumber numberWithBool:message.disableForwarding] forKey:@"allow-forward"];
        [dictionary setValue:[message.previewImage base64EncodedString] forKey:@"preview"];
    
    return [dictionary autorelease];
}

+ (BOOL) isNilOrEmpty:(NSString *) string
{
    BOOL isNilEmpty = YES;
    
    if (string != Nil && ![string isEqualToString:@""] )
    {
        isNilEmpty = NO;
    }
    
    return isNilEmpty;
}

/**
 * Take a populated dictionary that was parsed from JSON
 * and send it to the KikAPIPlugin.
 */
+ (KikAPIMessage *) KikAPIMessageFromDict:(NSDictionary *) dictionary
{
    KikAPIMessage * p_message = [KikAPIMessage message];
    
    NSString * title = [dictionary valueForKey:@"title"];
    NSString * description = [dictionary valueForKey:@"description"];
    NSArray * genericUris = [dictionary valueForKey:@"genericURIs"];
    NSArray * androidUris = [dictionary valueForKey:@"androidURIs"];
    NSArray * iphoneUris = [dictionary valueForKey:@"iphoneURIs"];
    NSString * filePath = [dictionary valueForKey:@"filePath"];
    NSDictionary * extras = [dictionary valueForKey:@"extras"];
    NSString * s_preview = [dictionary valueForKey:@"preview"];
    NSString * previewURL = [dictionary valueForKey:@"previewURL"];
    
    if (![KikPlugin isNilOrEmpty:title])
        p_message.title = title;
    
    if (![KikPlugin isNilOrEmpty:description])
        p_message.description = description;
    
    if (genericUris && [genericUris isKindOfClass:[NSArray class]] && [genericUris count] > 0 )
        p_message.genericURIs = [NSArray arrayWithArray:genericUris];
    
    if (androidUris && [androidUris isKindOfClass:[NSArray class]] &&  [androidUris count] > 0 ) {
        p_message.androidURIs = [NSArray arrayWithArray:androidUris];
    }
    
    if (iphoneUris && [iphoneUris isKindOfClass:[NSArray class]] && [iphoneUris count] > 0 ) {
        p_message.iphoneURIs = [NSArray arrayWithArray:iphoneUris];
    }
    
    if ( extras && [extras count] > 0 ) { 
        p_message.extras = [NSDictionary dictionaryWithDictionary:extras];
    }
    
    if (![KikPlugin isNilOrEmpty:filePath])
    {
        p_message.filePath = filePath;
    }
    
    if (![KikPlugin isNilOrEmpty:s_preview])
    {
        p_message.previewImage = [NSData dataFromBase64String:s_preview];
    } else if (![KikPlugin isNilOrEmpty:previewURL]) {
        p_message.previewImage = [NSData dataWithContentsOfFile:previewURL];
    }
    
    return p_message;
}
        

- (void) sendMessage:(NSMutableArray *) args withDict:(NSMutableDictionary *) options
{
    
    if([args count]==2 || [args count] == 3)
    {     
        //The first argument in the arguments parameter is the callbackID.
        //We use this to send data back to the successCallback or failureCallback
        //through PluginResult.   
        self.callbackID = [args pop];
        
        NSString * void_dict = [args pop];
        void_dict = [void_dict stringByReplacingOccurrencesOfString:@"\\\"" withString:@"\""];
        
        NSDictionary * dict = [void_dict JSONValue];
        
        if ([dict isKindOfClass:[NSDictionary class]] ) {
            NSString * conversationID = Nil;
            
            if ( [args count] == 1 ) {
                conversationID = [args pop];
            }
            
            // We are going to register as a kik handler
            // and we are going to suppress the app button if there was no call 
            // to initialize the kik plugin
            if ( !_initialized ) {
                [self registerAsKikHandler:@"" addAppButton:NO];
            }
            
            KikAPIMessage * message = [KikPlugin KikAPIMessageFromDict:dict];
            [KikAPIClient sendMessage:message toConversation:conversationID];
            
            // Now we can format a response to PhoneGap
            PluginResult * pluginResult = [PluginResult resultWithStatus:PGCommandStatus_OK];
            [self writeJavascript: [pluginResult toSuccessCallbackString:self.callbackID]];
        } else { 
            // Oops, Something has shit in the bed
            PluginResult * pluginResult = [PluginResult resultWithStatus:PGCommandStatus_ERROR];
            [self writeJavascript: [pluginResult toErrorCallbackString:self.callbackID]];
            return;
        }
    } else {
        PluginResult * pluginResult = [PluginResult resultWithStatus:PGCommandStatus_ERROR];
        [self writeJavascript: [pluginResult toErrorCallbackString:self.callbackID]];
    }
    return;
}

-(void) hasMessage:(NSMutableArray *) args withDict:(NSMutableDictionary *) options
{
    // We are simply going to create the new plugin result and return on iOS
    // this function is needed so there can be a common javascript file for both android and iOS
    self.callbackID = [args pop];
    PluginResult * pluginResult = [PluginResult resultWithStatus:PGCommandStatus_OK];
    [self writeJavascript: [pluginResult toSuccessCallbackString:self.callbackID]];
}


@end
