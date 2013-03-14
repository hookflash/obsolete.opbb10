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

#import <Foundation/Foundation.h>
#import <OpenpeerSDK/HOPProtocols.h>

@class Contact;
@class Session;
@class HOPConversationThread;
@class HOPMessage;

@interface SessionManager : NSObject

@property (retain) NSMutableDictionary* sessionsDictionary;


+ (id) sharedSessionManager;

- (Session*) createSessionForContact:(Contact*) contact;
- (Session*) createSessionForContacts:(NSArray*) contacts andConversationThread:(HOPConversationThread*) inConversationThread;
- (Session*) getSessionForContact:(Contact*) contact;
- (void) endSession:(Session*) session;

- (void) makeCallForSession:(Session*) inSession includeVideo:(BOOL) includeVideo;
- (void) answerCallForSession:(Session*) inSession;
- (void) endCallForSession:(Session*) inSession;

- (void) handleIncomingCall:(HOPCall*) call forSession:(Session*) inSession;

- (void) sendMessage:(NSString*) message forSession:(Session*) inSession;
- (void) onMessageReceived:(HOPMessage*) message forSessionId:(NSString*) sessionId;
@end
