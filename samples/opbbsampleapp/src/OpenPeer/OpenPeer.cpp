/*
 
 Copyright (c) 2012, SMB Phone Inc.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 
 */

#import "OpenPeer.h"
#import "OpenPeerUser.h"
#import "Utility.h"
//SDK
#import "OpenpeerSDK/HOPStack.h"
//Managers
#import "LoginManager.h"
//Delegates
#import "StackDelegate.h"
#import "MediaEngineDelegate.h"
#import "ConversationThreadDelegate.h"
#import "CallDelegate.h"
#import "ProvisioningAccountDelegate.h"
//View controllers
#import "MainViewController.h"

//Private methods
@interface OpenPeer ()

- (void) createDelegates;

@end


@implementation OpenPeer

/**
 Retrieves singleton object of the Open Peer.
 @return Singleton object of the Open Peer.
 */
+ (id) sharedOpenPeer
{
    static dispatch_once_t pred = 0;
    __strong static id _sharedObject = nil;
    dispatch_once(&pred, ^{
        _sharedObject = [[self alloc] init];
    });
    return _sharedObject;
}

/**
 Method used initialization of open peer stack. After initialization succeeds, login screen is showed.
 @param inMainViewController MainViewController Input main view controller.
 */
- (void) prepareWithMainViewController:(MainViewController *)inMainViewController
{
    self.mainViewController = inMainViewController;
    
    //Set log levels and start logging
    [self startLogger];
    
    //Created all delegates required for openpeer stack initialization.
    [self createDelegates];

    //Init openpeer stack and set created delegates
    BOOL prepared = [[HOPStack sharedStack] initStackDelegate:self.stackDelegate mediaEngineDelegate:self.mediaEngineDelegate conversationThreadDelegate:self.conversationThreadDelegate callDelegate:self.callDelegate userAgent:[Utility getUserAgentName] deviceOs:[Utility getDeviceOs] platform:[Utility getPlatform]];
    
    //If openpeer stack is created, start with login procedure and display login view
    if (prepared)
    {
        [[LoginManager sharedLoginManager] login];
    }
}

/**
 Method used for all delegates creation. Delegates will catch events from the Open Peer SDK and handle them properly.
 */
- (void) createDelegates
{
    self.stackDelegate = [[[StackDelegate alloc] init] autorelease];
    self.mediaEngineDelegate = [[[MediaEngineDelegate alloc] init] autorelease];
    self.conversationThreadDelegate = [[[ConversationThreadDelegate alloc] init] autorelease];
    self.callDelegate = [[[CallDelegate alloc] init] autorelease];
    self.provisioningAccountDelegate = [[[ProvisioningAccountDelegate alloc] init] autorelease];
}

/**
 Method used for setting log levels and starting logger.
 */
- (void) startLogger
{
    //For each system you can choose log level from HOPClientLogLevelNone (turned off) to HOPClientLogLevelTrace (most detail).
    [HOPStack setLogLevel:HOPClientLogLevelNone];
    [HOPStack setLogLevelbyName:@"hookflash_gui" level:HOPClientLogLevelNone];
    [HOPStack setLogLevelbyName:@"hookflash" level:HOPClientLogLevelNone];
    [HOPStack setLogLevelbyName:@"hookflash_services" level:HOPClientLogLevelNone];
    [HOPStack setLogLevelbyName:@"zsLib" level:HOPClientLogLevelNone];
    [HOPStack setLogLevelbyName:@"hookflash_services_http" level:HOPClientLogLevelNone];
    [HOPStack setLogLevelbyName:@"hookflash_stack_message" level:HOPClientLogLevelNone];
    [HOPStack setLogLevelbyName:@"hookflash_stack" level:HOPClientLogLevelNone];
    [HOPStack setLogLevelbyName:@"hookflash_webrtc" level:HOPClientLogLevelNone];
    //Srart logger without colorized output
    [HOPStack installStdOutLogger:NO];
}
@end
