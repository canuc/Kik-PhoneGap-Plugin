//
//  KikAPI.h
//  kikapilib
//
//  Created by Nick Belyaev on 11-03-28.
//  Copyright 2011 Kik Interactive Inc. All rights reserved.
//

/**
 * @mainpage
 * Basic steps to get started using Kik to share your applications' content:
 * -# Add the binary (libkikapilib.a) and header (KikAPI.h) to your project
 * -# Register as a Kik plugin when your application launches:
 * <pre><code>-(void)applicationDidFinishLaunching {
 *      // Set addAppButton to YES if you want your application to be displayed in Kiks' media tray
 *      [KikAPI registerAsKikPluginWithAppID:@"your apps' ID" withHomepageURI:@"http://www.myhomepage.com" addAppButton:YES];
 * </code></pre>
 * -# In your applications' URL handler, retrieve the context and data that your application was launched with:
 * <pre><code>-(BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
 *      // This object will contain all the data you need for your application to construct, view and/or send content
 *      KikAPIData *myData = [KikAPI handleOpenURL:url];
 *      ...
 * }</code></pre>
 * -# Determine the behaviour of your program based on the launch context:
 * <pre><code>// It is very important to hold onto this! You can use the conversation ID to automatically direct the content to the
 * // correct conversation.
 * NSString *conversationID = myData.conversationID;
 * // Determine whether we are viewing or selecting content:
 * if (myData.type == KikAPIDataTypeView) {
 *      // ...
 * } else if (myData.type == KikAPIDataTypePick) {
 *      // ...
 * } else if (myData.type == KikAPIDataTypeNotKik) {
 *      // ...
 * }
 * </code></pre>
 * For more information on the type property, @see KikAPIData
 * -# If you are going to send content, instantiate and populate a KikAPIMessage object:
 * <pre><code>KikAPIMessage *myMessage = [KikAPIMessage message];
 * myMessage.title = @"My title";
 * myMessage.description = @"My description";
 * myMessage.previewImage = myImage;
 * myMessage.filePath = myFilePath;
 * myMessage.iphoneURIs = [NSArray arrayWithObjects:@"my iphone URI", nil];
 * myMessage.androidURIs = [NSArray arrayWithObjects:@"my android URI", nil];
 * myMessage.genericURIs = [NSArray arrayWithObjects:@"my generic URI", nil];
 * </code></pre>
 * For more information on these properties, @see KikAPIMessage
 * -# Finally, send the message off to Kik!
 * <pre><code>[KikAPI sendMessage:myMessage toConversation:conversationID];
 * // We're done! It was that easy!
 * <code></pre>
 */

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#pragma mark -

/**
 * @brief The actual message that is sent through Kik from your application
 */
@interface KikAPIMessage

/**
 * The title for your content message.
 */
@property (nonatomic, copy)		NSString		*title;

/**
 * The description for your content message. This is displayed as preview text in the message bubble.
 */
@property (nonatomic, copy)		NSString		*description;

/**
 * You can choose a thumbnail preview image to be displayed with your message. This property stores the encoded 
 * (ie. jpeg, png) data representing this image. You can also set your preview image with a UIImage object, using the
 * setPreviewFromImage method. Image scaling is automatically performed by Kik to fit within the dimensions of the message
 * bubble.
 * @see setPreviewFromImage
 */
@property (nonatomic, retain)	NSData			*previewImage;

/**
 * Sets the previewImage property using a UIImage object instead of an encoded byte array, for convenience.
 * @param image UIImage to be set as previewImage
 */
- (void)setPreviewFromImage:(UIImage *)image;

/**
 * A dictionary of extra key-value string pairs to be sent with the message.
 * This information is not read by Kik and is only relevant to your application.
 * For example, a message containing information about a book could have extras like this:
 * <pre><code>myMessage.extras = [NSDictionary dictionaryWithObjectsAndKeys:@"author", @"Mark Twain", @"pages", @"435", nil];</code></pre>
 */
@property (nonatomic, retain)	NSDictionary	*extras;

/**
 * If the recipient is running Kik on an Android device, but does not have your application, these URIs will attempt to launch in the order
 * that they are placed in the array. It is recommended that the first URI in the array be an address from which the user can download your application
 * (ie. market://details?id=com.kik.myapplication). It is important to note that your application should be able to open URIs of the scheme 
 * kik-<YOUR_APP_ID>://
 */
@property (nonatomic, retain)	NSArray			*androidURIs;

/**
 * If the recipient is running Kik on an iPhone, but does not have your application, these URIs will attempt to launch in the order
 * that they are placed in the array. It is recommended that the first URI in the array be an address from which the user can download your application
 * (ie. http://itunes.com/apps/my-application/...). It is important to note that your application should be able to open URIs of the scheme 
 * kik-<YOUR_APP_ID>://
 */
@property (nonatomic, retain)	NSArray			*iphoneURIs;

/**
 * If the receiving client can't open any of the iPhone URIs (for iOS) or android URIs (for Android),
 * these generic URIs will attempt to launch in the order that they are placed in the array. It is recommended that you link to a web
 * application that can open your content or a site where you can download the application.
 */
@property (nonatomic, retain)	NSArray			*genericURIs;

/**
 * To send a message with a file attachment, assign a valid local file path to this property.
 * @see fileUrl
 */
@property (nonatomic, copy)		NSString		*filePath;

/**
 * Set automatically by Kik if a file is attached. When opening content in your application,
 * an attached file can be downloaded via the URL contained in this property.
 * @see filePath
 */
@property (nonatomic, readonly)		NSString		*fileUrl;

/**
 * Specifies whether or not Kik should allow forwarding of your content message.
 */
@property (nonatomic, assign)	BOOL			disableForwarding;

/**
 * For convenience, you can call this constructor to receive an autoreleased instance of KikAPIMessage.
 * @return instance of KikAPIMessage
 */
+ (KikAPIMessage *)message;

@end


#pragma mark -

/**
 * These represent the different contexts that your application can be launched with.
 * @see type
 */
typedef enum KikAPIDataType
{
	KikAPIDataTypeView		= 1,
	KikAPIDataTypePick		= 2,
	KikAPIDataTypeNotKik	= 3
} KikAPIDataType;


/**
 * @brief Contains the actual message and the context that our application was launched
 * with
 */
@interface KikAPIData

/**
 * If we are viewing a message from our application, this property will contain the actual content.
 */
@property (nonatomic, retain)	KikAPIMessage	*message;

/**
 * Your app can be launched in 2 modes: viewer and picker. When a user clicks on the content
 * message, your app will be launched in the "viewer" mode. If your application registers as a Kik plugin, it will be 
 * displayed in the media tray in Kik. If you compose a message by selecting your application in the media tray, it will
 * launch in "picker" mode. This is where you select content to be sent and return back to Kik to send the message. 
 * If your app is launched from outside of Kik (e.g. from the home screen or URL handler), this will be set to KikAPIDataTypeNotKik.
 */
@property (nonatomic, assign)	KikAPIDataType	type;

/**
 * Conversation ID for which content has been requested.
 * When sending content back to Kik, include this ID in the message so that the user doesn't have to respecify which conversation to send
 * the content to.
 */
@property (nonatomic, copy)		NSString		*conversationID;

/**
 * Specifies if the clicked content message is outgoing (originating from the user of this phone).
 */
@property (nonatomic, assign)	BOOL			isOutgoing;

/**
 * Specifies current user's name (phone owner) as set on Kik settings screen.
 */
@property (nonatomic, copy)		NSString		*myName;

/**
 * Specifies first name of the sender of the content message.
 */
@property (nonatomic, copy)		NSString		*senderName;

@end




#pragma mark -

/**
 * @brief Static class that facilitates communication between Kik and your application
 *
 * KikAPIClient is a 'static' class that will allow easily send content to the Kik app.
 * The easiest way to start using the Kik API is to use the helper constructor for KikAPIMessage
 * and pass it to the "sendMessage" method, e.g.:
 * <pre><code>
 * [KikAPIClient sendMessage:[KikAPIMessage message]];
 * </code></pre>
 *
 * Although this will work, it's useless since we are not passing any information to send, so instead we should
 * populate the message properties before sending to Kik:
 * <pre><code>
 * KikAPIMessage *myMessage = [KikAPIMessage message];
 * myMessage.title = @"Hello world";
 * myMessage.description = @"This is my first Kik message";
 * [myMessage setPreviewFromImage:[UIImage imageNamed:@"resource_name_of_my_image.png"]];
 * myMessage.androidURIs = [[[NSArray alloc] initWithObjects:@"market://details?id=com.kik.myapplication", nil] autorelease];
 * myMessage.iphoneURIs = [[[NSArray alloc] initWithObjects:@"http://itunes.com/apps/myapplication/38492348923", nil] autorelease];
 * myMessage.genericURIs = [[[NSArray alloc] initWithObjects:@"http://www.mysite.com/myapplication", nil] autorelease];
 *
 * // Remember to hold onto the conversation ID so that you can automatically send the message to the right conversation
 * [KikAPIClient sendMessage:myMessage toConversation:conversationID];
 * </code></pre>
 */
@interface KikAPIClient

/**
 * To achieve successful communication between Kik and your app, you need to call this method when your
 * app first starts (i.e. didFinishLaunchingWithOptions delegate method).
 * @param appID your application ID; you should use the same app ID across platforms.
 *        Reverse DNS notation will work the best, e.g: com.mycompany.myapp
 * @param homepageURI string that will be used as the default URI to launch when user clicks the bubble and all other
 *        URIs specified for the content message have failed to open. Use your company's homepage for this property (as opposed to
 *        an App store or Android Market link).
 * @param addAppButton BOOL specifying whether or not you want your app's button to be listed in the Kik
 *        content picker (clicking on it will cause your app being launched in "picker" mode).
 */
+ (void)registerAsKikPluginWithAppID:(NSString *)appID withHomepageURI:(NSString *)homepageURI addAppButton:(BOOL)addAppButton;


/**
 * Sends a content message through Kik. The conversation picker will be presented to choose 
 * who we want to send the message to. The parameterized version of this method can be called if the desired conversation
 * is already known.
 * @param message content message to be sent
 */
+ (void)sendMessage:(KikAPIMessage *)message;


/**
 * Send content message to an existing conversation.
 * @param message content message to be sent
 * @param conversationID conversation to which we wish to send the content message
 */
+ (void)sendMessage:(KikAPIMessage *)message toConversation:(NSString *)conversationID;


/**
 * Given the url parameter passed into the app by the app delegate, this method will return a KikAPIData object
 * containing details about what Kik has requested. This method directly mimics the app delegate method and
 * its parameters (from iOS 4.0.1). To use it, add it to the corresponding app delegate implementation.
 * <pre>
 * -(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
 *     KikAPIData *kikData = [KikAPIClient handleOpenURL:url sourceApplication:sourceApplication annotation:annotation];
 *     if (kikData.type == KikAPIDataTypePick) {
 *          // Code to handle selection of content to send
 *     } else if (kikData.type == KikAPIDataTypeView) {
 *          // Code to handle viewing of received content from within your application
 *     } else if (kikData.type == KikAPIDataTypeNotKik) {
 *          // Code to handle a launch from outside of Kik
 *     }
 * }
 * </pre>
 * @param url url passed to app delegate
 * @param source source application bundle id passed to your app delegate
 * @param annotation userinfo containing custom information passed to your app delegate
 * @return KikAPIData object containing the context and data which your application was launched with. It is
 * important to note that this object contains a conversationID that is used to automatically select the 
 * correct conversation to send your message to.n
 * @see handleOpenURL:
 */
+ (KikAPIData *)handleOpenURL:(NSURL *)url sourceApplication:(NSString *)source annotation:(id)annotation;


/**
 * Similar to handleOpenURL:sourceApplication:annotation: except that it's designed for devices with
 * older iOS versions (3-4), in which no additional information is passed on the the app delegate.
 * @param url url passed on to the app delegate
 * @return KikAPIData object containing the context and data which your application was launched with. It is
 * important to note that this object contains a conversationID that is used to automatically select the 
 * correct conversation to send your message to.
 * @see handleOpenURL:sourceApplication:annotation:
 */
+ (KikAPIData *)handleOpenURL:(NSURL *)url;

@end




