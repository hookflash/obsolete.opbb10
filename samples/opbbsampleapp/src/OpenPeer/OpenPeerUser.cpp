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

#import <Foundation/NSKeyedArchiver.h>
#import <Foundation/Foundation.h>

#import "OpenPeerUser.h"
//SDK
#import <OpenpeerSDK/HOPProvisioningAccount.h>
//Utility
#import "XMLWriter.h"
#import "Constants.h"


//Private methods
@interface OpenPeerUser()

- (id) initSingleton;

@end

@implementation OpenPeerUser

/**
 Retrieves singleton object of the Open Peer User.
 @return Singleton object of the Open Peer User.
 */
+ (id) sharedOpenPeerUser
{
    static dispatch_once_t pred = 0;
    __strong static id _sharedObject = nil;
    dispatch_once(&pred, ^{
        _sharedObject = [[self alloc] initSingleton];
    });
    return _sharedObject;
}

/**
 Initialization of Open Peer User singleton object.
 @return Object of the Open Peer User.
 */
- (id) initSingleton
{
    self = [super init];
    
    if (self)
    {
        NSData* data = [[NSUserDefaults standardUserDefaults] objectForKey:keyOpenPeerUser];
        if (data)
        {
            NSKeyedUnarchiver *aDecoder = [[[NSKeyedUnarchiver alloc] initForReadingWithData:data] autorelease];
            
            self.userId = [aDecoder decodeObjectForKey:archiveUserId];
            self.contactId = [aDecoder decodeObjectForKey:archiveContactId];
            self.accountSalt = [aDecoder decodeObjectForKey:archiveAccountSalt];
            self.passwordNonce = [aDecoder decodeObjectForKey:archivePasswordNonce];
            self.privatePeerFile = [aDecoder decodeObjectForKey:archivePrivatePeerFile];
            self.peerFilePassword = [aDecoder decodeObjectForKey:archivePeerFilePassword];
            self.lastProfileUpdateTimestamp = [aDecoder decodeDoubleForKey:archiveLastProfileUpdateTimestamp];
            
            [aDecoder finishDecoding];
        }
    }
    return self;
}

/**
 Saves user information on local device.
 */
- (void) saveUserData
{
    self.userId = [[HOPProvisioningAccount sharedProvisioningAccount] getUserID];
    self.contactId = [[HOPProvisioningAccount sharedProvisioningAccount] getContactID];
    self.accountSalt = [[HOPProvisioningAccount sharedProvisioningAccount] getAccountSalt];
    self.passwordNonce = [[HOPProvisioningAccount sharedProvisioningAccount] getPasswordNonce];
    self.privatePeerFile = [[HOPProvisioningAccount sharedProvisioningAccount] getPrivatePeerFile];
    self.peerFilePassword = [[HOPProvisioningAccount sharedProvisioningAccount] getPassword];
    self.lastProfileUpdateTimestamp = [[HOPProvisioningAccount sharedProvisioningAccount] getLastProfileUpdatedTime];
    
    NSMutableData *data = [NSMutableData data];
    NSKeyedArchiver *aCoder = [[[NSKeyedArchiver alloc] initForWritingWithMutableData:data] autorelease];
    [aCoder encodeObject:self.userId forKey:archiveUserId];
    [aCoder encodeObject:self.contactId forKey:archiveContactId];
    [aCoder encodeObject:self.accountSalt forKey:archiveAccountSalt];
    [aCoder encodeObject:self.passwordNonce forKey:archivePasswordNonce];
    [aCoder encodeObject:self.privatePeerFile forKey:archivePrivatePeerFile];
    [aCoder encodeObject:self.peerFilePassword forKey:archivePeerFilePassword];
    [aCoder encodeDouble:self.lastProfileUpdateTimestamp forKey:archiveLastProfileUpdateTimestamp];
    
    [aCoder finishEncoding];
    
    [[NSUserDefaults standardUserDefaults] setObject:data forKey:keyOpenPeerUser];
}

/**
 deletes user information from local device.
 */
- (void) deleteUserData
{
    self.userId = nil;
    self.contactId = nil;
    self.accountSalt = nil;
    self.passwordNonce = nil;
    self.privatePeerFile = nil;
    self.peerFilePassword = nil;
    self.lastProfileUpdateTimestamp = 0;
    
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:keyOpenPeerUser];
}

- (NSString*) createProfileBundle
{
    NSString* ret = nil;
    
    XMLWriter *xmlWriter = [[XMLWriter alloc] init];
    [xmlWriter writeStartElement:profileXmlTagProfile];
    
    [xmlWriter writeStartElement:profileXmlTagName];
    [xmlWriter writeCharacters:self.fullName];
    [xmlWriter writeEndElement];
    
    [xmlWriter writeStartElement:profileXmlTagContactID];
    [xmlWriter writeCharacters:self.contactId];
    [xmlWriter writeEndElement];
    
    [xmlWriter writeStartElement:profileXmlTagUserID];
    [xmlWriter writeCharacters:self.userId];
    [xmlWriter writeEndElement];
    
    [xmlWriter writeStartElement:profileXmlTagIdentities];
        
        [xmlWriter writeStartElement:profileXmlTagIdentityBundle];
        
        [xmlWriter writeStartElement:profileXmlTagIdentity];
        [xmlWriter writeCharacters:[self.providerKey stringValue]];
        [xmlWriter writeEndElement];
        
        [xmlWriter writeStartElement:profileXmlTagSocialId];
        [xmlWriter writeCharacters:self.contactProviderId];
        [xmlWriter writeEndElement];
        
        [xmlWriter writeEndElement];

    [xmlWriter writeEndElement];
    
    [xmlWriter writeEndElement];
    
    ret = [NSString stringWithString: [xmlWriter toString]];
    [xmlWriter release];
    return ret;
}
@end
