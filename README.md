Kik API PhoneGap Plugin
================================

This plugin will allow you to send content messages from PhoneGap applications on iPhone and android.

This Plugin is covered under the MIT Licence, except for the KikApi.jar, and kikapilib.a the licences for which can be found at: http://kik.com/dev/TOS.php

Getting Started
===============

Download the latest version of PhoneGap from www.phonegap.com.

Set up the PhoneGap Development environment for iOS and Android. 


Check out the /example/www/index.html to see how it works.

<pre>
|-www
|  `-KikAPI.js
|-natve
|  |-android
|  |-ios

</pre>

/www/KikAPI.js This is the javascript wrapper for the KikAPI, copy this into your own project www project directory

/native/android | ios These directories contain all the native code. These files should be placed into ther plugins directory.

This plugin currently supports Sending platform messages that contain extras, description text, and title, as well as preview image and file attachment.

The Kik API libraries are each changing and this is a current snapshot of the KikAPI v1.


Android
===============

Create a basic PhoneGap Android application. See http://www.phonegap.com/start/#android

In the PhoneGap Andoroid application you will need to put the following in your /res/xml/plugins.xml file:

<pre>
&lt;plugin name="KikPhoneGap" value="com.kik.phonegap.plugin.messenger.KikMessengerPlugin" /&gt;
</pre>

http://wiki.phonegap.com/How%20to%20Install%20a%20PhoneGap%20Plugin%20for%20Android

From the PhoneGap Kik Plugin folder copy the contents of the /native/android/ folder into the root of your PhoneGap Android application, add the Kik Android SDK to the build path.

From the PhoneGap Kik Plugin folder copy the /www/KikAPI.js files into your /assets/www/ folder.


Android Intents
===============

You will have to add the following to your AndroidManifest.xml
<pre>
&lt;intent-filter&gt;	
	&lt;action android:name=kik.request" /&gt;
      	&lt;category android:name="android.intent.category.DEFAULT" /&gt;
&lt;/intent-filter&gt;
&lt;intent-filter&gt;	
      	&lt;action android:name="android.intent.action.VIEW"/&gt;
      	&lt;category android:name="android.intent.category.DEFAULT"/&gt;
        &lt;data android:scheme="kik-com.kik.phonegap.KikAPIPhoneGap"/&gt;
&lt;/intent-filter&gt;
</pre>

The first intent filter allows the project to receive CHOOSE events from kik. A CHOOSE event is when the user launches your application from the Kik Media Tray.

The second allows you to receive content messages that are sent from another platform.

iOS (Mac OS X)
===============

1. Get the latest iOS project from [http://github.com/phonegap/phonegap-iphone](http://github.com/phonegap/phonegap-iphone) and read the README there about getting started with iOS.
2. Create a new Xcode project from the PhoneGap template that you created and installed (you did that if you read the README on github I hope).
3. From the **PhoneGap Kik Plugin** folder copy the contents of the **native/ios** folder into your app in Xcode (usually in the **Plugins** folder group, make sure to include the library). Make sure it is added in as a folder reference.
4. Find the PhoneGap.plist file in the project navigator, expand the "Plugins" sub-tree, and add a new entry. For the key, add **KikPhoneGap**, and its value will be **KikPlugin**
5. From the **PhoneGap Kik Plugin** folder copy the contents of the **www** folder into the **www** directory don't forget to add the
6. From the **PhoneGap Kik Plugin** folder copy the contents of the **lib** folder into the **www** directory in Xcode (don't forget to add script tags in your index.html to reference any .js files copied over)
7. for Xcode 4, you will need to build it once, and heed the warning - this is an Xcode 4 template limitation. The warning instructions will tell you to drag copy the **www** folder into the project in Xcode (add as a **folder reference** which is a blue folder).
8. Under the group **Supporting Files**, find your **[PROJECTNAME]-Info.plist**, right-click on the file and select **Open As -> Source Code**, add the **URL Scheme** from the section below


iOS URL Scheme
-----------

Make sure you add the scheme to your [PROJECTNAME]-Info.plist (located as one of the files in your Xcode project), substitute [APP_ID] and [SCHEME_ID] below to the appropriate values. This is to handle requests to open Content Messages by your application


* [**APP_ID**] You should use you bundle identifier that is found in the project plist file.

<pre>
&lt;key&gt;CFBundleURLTypes&lt;/key&gt;
&lt;array&gt;
	&lt;dict&gt;
		&lt;key&gt;CFBundleURLName&lt;/key&gt;
		&lt;string&gt;kik&lt;/string&gt;
		&lt;key&gt;CFBundleURLSchemes&lt;/key&gt;
		&lt;array&gt;
			&lt;string&gt;kik-[APP_ID]&lt;/string&gt;
		&lt;/array&gt;
	&lt;/dict&gt;
&lt;/array&gt;
</pre>
