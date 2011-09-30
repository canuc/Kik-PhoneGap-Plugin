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

/**
 * This can be used to see if a previewURL should be used or not
 *
 *
 */
function supportsToDataURL()
{
	var c = document.createElement("canvas");
	var data = c.toDataURL("image/png");
	return (data.indexOf("data:image/png") == 0);
}

function KikAPIMessage () {
    this.title = "";
    this.description = "";
    this.genericURIs = new Array();
    this.iphoneURIs = new Array();
    this.androidURIs = new Array();
    this.extras = new Object();
    this.fileLocation = "";
    this.preview = ""; // The preview will be rescaled if it is too large
    this.fileUrl = "";
    // This is a quirk for android, we are not able to get the image data that we need
    // this is the url for the preview
    this.previewUrl = "";
    this.setPreviewImage = function(img) 
    {
     	if (supportsToDataURL()) {
        	var canvas = document.createElement("canvas");
        	canvas.width = img.width;
        	canvas.height = img.height;
        	var ctx = canvas.getContext("2d");
       		ctx.drawImage(img, 0, 0);
        	var dataURL = canvas.toDataURL("image/jpg");
        	// We want to chop of the begginging of the base 64 string so that we can send it 
        	// sanitarily
        	this.preview = dataURL.replace(/^data:image\/(png|jpg);base64,/, "");
     	}
    };
    
    /*
     * This will send the data in types, it will have the success and fail callbacks
     */
    this.addExtra = function ( key,val ) {
        this.extras[key] = val;
    };
    
    return this;
}

function KikAPIData () {
    this.message = new KikAPIMessage();
    this.conversationID = "";
    // This will represent the invocation method:
    // 1 - KikAPIDataTypeView - This will be if we have selected a type and need to view the data
    // 2 - KikAPIDataTypePick - Thit will be if we need to pick new content
    this.type = 0; 
    this.isOutgoing = false;
    this.myName = "";
    this.senderName = "";
}

/*
 * Here we are going to setup the kik API to recieve success and failure states
 */
KikAPIClient = {
    //
    // Overide this function to provide functionality for when an incoming message is opened with this application.
    // APIData will be a new API data object where its elements can be retrieved.
    // 
    message_read_ready : function ( apiData ) {
        return;
    },
    // WARNING: 
    // do not reassign this. Will result in inexpected KikAPI Behaviour.
    message_ready : function ( in_message ) 
    {
        // Here we will take a JSON object and we will read its values
        var api_message = new KikAPIMessage();
        var message_data = JSON.parse(in_message);
        
        var APIData = new KikAPIData();
        APIData.conversationID = message_data.message.conversationID;
        APIData.type = message_data.message.type;
        if ( APIData.type == 1 ) {
            APIData.myName = message_data.message.myName;
            APIData.senderName = message_data.message.senderName;
            //
            // Now extract the relevant message data
            //
            api_message.title = message_data.message.message.title;
            api_message.description = message_data.message.message.description;
            api_message.genericURIs = message_data.message.message.genericURIs;
            api_message.androidURIs = message_data.message.message.androidURIs;
            api_message.iphoneURIs = message_data.message.message.iphoneURIs;
            api_message.fileUrl = message_data.message.message.fileUrl;
            api_message.extras = message_data.message.message.extras;
            
            // Set the APIData
            APIData.message = api_message;
        }
        
        // This can be overiden to notify the callback when the message is available
        this.message_read_ready ( APIData );
        
        return;
    },
    // This may be overiden to provide support for a success callback
    success_callback : function( result ) {
        // do nothing for now
    },
    // This can be called to provide a failure callback
    failure_callback : function ( result ) {
        alert( "Error with Kik API!!" );
    },
    // Initialization should always be done on both platforms
    init: function( url, shouldShowButton, appID ) {
        
        var initializationData = new Array();
        initializationData[0] = url;
        
        if ( shouldShowButton == false )
        initializationData[1] = 0;
        else 
        initializationData[1] = 1;
        
        initializationData[2] = appID;
        
        return PhoneGap.exec(this.success_callback,this.failure_callback ,"KikPhoneGap", "init", initializationData);
    },
    // This will send the data to the KikAPI with a KikAPIMessage object
    sendMessage: function( message, conversationID ) {
        var apiData = new Array();
        apiData[0] = JSON.stringify(message);
        apiData[1] = conversationID;
        return PhoneGap.exec(this.success_callback,this.failure_callback ,"KikPhoneGap", "sendMessage",apiData);
    },
    getMessage: function() {
     	alert("GET MESSAGE");
     	var apiData = new Array();
        return PhoneGap.exec(this.success_callback,this.failure_callback ,"KikPhoneGap", "hasMessage",apiData);
    }
}


PhoneGap.addConstructor(function() {
    PhoneGap.addPlugin("kik", KikAPIClient);
});