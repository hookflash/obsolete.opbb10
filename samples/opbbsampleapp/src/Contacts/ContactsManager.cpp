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

#include "Contacts/ContactsManager.h"

using namespace hookflash::blackberry::app;

boost::shared_ptr<ContactsManager> ContactsManager::sSharedManager;

/*
#import "MainViewController.h"
#import "ContactsTableViewController.h"
#import "OpenPeer.h"
#import "OpenPeerUser.h"
#import "Contact.h"
#import "SBJsonParser.h"
#import <OpenpeerSDK/HOPProvisioningAccount.h>
#import <OpenpeerSDK/HOPProvisioningAccountIdentityLookupQuery.h>
#import <OpenpeerSDK/HOPIdentity.h>
#import <OpenpeerSDK/HOPContact.h>
#import <OpenpeerSDK/HOPLookupProfileInfo.h>
#import <OpenpeerSDK/HOPProvisioningAccountPeerFileLookupQuery.h>

ContactsManager::ContactsManager() {
  keyJSONContacLastName     = L"firstName";
  keyJSONContactFirstName   = L"lastName";
  keyJSONContactId          = L"id";
  keyJSONContactProfession  = L"headline";
  keyJSONContactPictureURL  = L"pictureUrl";
  keyJSONContactFullName    = L"fullName";

  self.linkedinContactsWebView = [[[UIWebView alloc] init] autorelease];
  self.linkedinContactsWebView.delegate = self;

  self.mContacts = [[[NSMutableArray alloc] init] autorelease];
  self.contactsDictionaryByProvider = [[[NSMutableDictionary alloc] init] autorelease];
}
      //-----------------------------------------------------------------------
      // Retrieves singleton object of the Contacts Manager.
+ (id) sharedContactsManager
{
    static dispatch_once_t pred = 0;
    __strong static id _sharedObject = nil;
    dispatch_once(&pred, ^{
        _sharedObject = [[self alloc] initSingleton];
    });
    return _sharedObject;
}

      //-----------------------------------------------------------------------
      // Initialize singleton object of the Contacts Manager.
- (id) initSingleton
{
    self = [super init];
    if (self)
    {
    }
    return self;
}

      //-----------------------------------------------------------------------
      // Initiates contacts loading procedure.

- (void) loadContacts
{
    [[[OpenPeer sharedOpenPeer] mainViewController] showContactsTable];
    
    [[[[OpenPeer sharedOpenPeer] mainViewController] contactsTableViewController] onContactsLoadingStarted];
    
    //NSString* urlAddress = [NSString stringWithFormat:@"http://%@/%@", @"provisioning-stable-dev.hookflash.me", @"/api_web_res/liconnections.html"];
    NSString* urlAddress = [NSString stringWithFormat:@"http://%@/%@", @"provisioning-stable-dev.hookflash.me", @"/api_web_res/fbconnections.html"];
    
    NSURL *url = [NSURL URLWithString:urlAddress];
    
    //URL Requst Object
    NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];
    
    //Load the request in the UIWebView.
    [self.linkedinContactsWebView loadRequest:requestObj];
}

      //-----------------------------------------------------------------------
      // Web view which will perform contacts loading procedure.

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSString *requestString = [[request URL] absoluteString];
    if ([requestString hasPrefix:@"hookflash-js-frame:"]) {
        
        NSArray *components = [requestString componentsSeparatedByString:@":"];
        
        NSString *function = (NSString*)[components objectAtIndex:1];
        
        requestString = [requestString stringByReplacingOccurrencesOfString:function withString:@""];
        requestString = [requestString stringByReplacingOccurrencesOfString:(NSString*)[components objectAtIndex:0] withString:@""];
        requestString = [requestString stringByReplacingCharactersInRange:NSMakeRange(0, 2) withString:@""];
        
        NSString *params = [requestString stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        
        NSString *functionNameSelector = [NSString stringWithFormat:@"%@:", function];
        //Execute JSON parsing in function read from requestString.
        [self performSelector:NSSelectorFromString(functionNameSelector) withObject:params];
        return NO;
    }
    return YES;
}

      //-----------------------------------------------------------------------
      // Parse JSON to get the profile for logged user.
- (void)proccessMyProfile:(NSString*)input
{
    SBJsonParser *jsonParser = [[SBJsonParser alloc] init];
    NSError *error = nil;
    NSDictionary *result = [jsonParser objectWithString:input error:&error];
    [jsonParser release], jsonParser = nil;
    if (!error)
    {
        NSString *fullName = [[NSString stringWithFormat:@"%@ %@", [result objectForKey:keyJSONContactFirstName], [result objectForKey:keyJSONContacLastName]] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        if ([fullName length] > 0)
            [[OpenPeerUser sharedOpenPeerUser] setFullName:fullName];
        
        //[[[OpenPeer sharedOpenPeer] mainViewController].contactsNavigationController.navigationBar.topItem setTitle:[NSString stringWithFormat:@"%@ Contacts",fullName]];
        
        NSNumber* providerKey = [NSNumber numberWithInt:HOPProvisioningAccountIdentityTypeLinkedInID];
        if (providerKey)
            [[OpenPeerUser sharedOpenPeerUser] setProviderKey:providerKey];
        
        NSString* cotnactProviderId = [result objectForKey:keyJSONContactId];
        if ([cotnactProviderId length] > 0)
            [[OpenPeerUser sharedOpenPeerUser] setContactProviderId:cotnactProviderId];
    }
        
    NSString *jsMethodName = @"getAllConnections()";
    NSNumber *lastUpdateTimestamp = 0;//[[StorageManager storageManager] getLastUpdateTimestamp];
    if ([lastUpdateTimestamp intValue] != 0)
    {
        jsMethodName = [NSString stringWithFormat:@"getNewConnections(%@)", [lastUpdateTimestamp stringValue]];
    }
    
    [self.linkedinContactsWebView performSelectorOnMainThread:@selector(stringByEvaluatingJavaScriptFromString:) withObject:jsMethodName waitUntilDone:NO];
}

      //-----------------------------------------------------------------------
      // Process connections.

- (void)proccessConnections:(NSString*)input
{
    //Parse JSON to get the contacts
    SBJsonParser *jsonParser = [[SBJsonParser alloc] init];
    NSError *error = nil;
    NSArray *result = [jsonParser objectWithString:input error:&error];
    [jsonParser release], jsonParser = nil;
    
    if (!error)
    {
        NSNumber* providerKey = [NSNumber numberWithInt:HOPProvisioningAccountIdentityTypeLinkedInID];
        NSMutableDictionary* contacts = [[NSMutableDictionary alloc] init];
        
        for (NSDictionary* dict in result)
        {
           NSString* providerContactId = [dict objectForKey:keyJSONContactId];
           
           if (providerContactId)
           {
               NSString *fullName = [[NSString stringWithFormat:@"%@ %@", [dict objectForKey:keyJSONContactFirstName], [dict objectForKey:keyJSONContacLastName]] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
               if (fullName)
               {
                   NSString* profession = [dict objectForKey:keyJSONContactProfession];
                   NSString *avatarUrl = [dict objectForKey:keyJSONContactPictureURL];
                   
                   Contact* contact = [[Contact alloc] initWithFullName:fullName profession:profession avatarUrl:avatarUrl identityProvider:HOPProvisioningAccountIdentityTypeLinkedInID identityContactId:providerContactId];
            
                   [self.mContacts addObject:contact];
                   [contacts setObject:contact forKey:providerContactId];
                   [contact release];
               }
           }
        }
        [self.contactsDictionaryByProvider setObject:contacts forKey:providerKey];
        [contacts release];
    }
    
    [[[[OpenPeer sharedOpenPeer] mainViewController] contactsTableViewController] onContactsLoaded];
    
    [self contactsLookupQuery:self.mContacts];
    [[[[OpenPeer sharedOpenPeer] mainViewController] contactsTableViewController] onContactsPeerFilesLoadingStarted];
}

      //-----------------------------------------------------------------------
      // Check contact identites against openpeer database.

- (void)contactsLookupQuery:(NSArray *)contacts
{
    NSMutableArray* identities = [[NSMutableArray alloc] init];
    
    
    for (Contact* contact in self.mContacts)
    {
        //Add all associated contact identities
        for (HOPIdentity* identity in contact.identities)
        {
            [identities addObject:identity];
        }
        
    }
    
    [[HOPProvisioningAccount sharedProvisioningAccount] identityLookup:self identities:identities];
    
    [identities release];
}

      //-----------------------------------------------------------------------
      // Does JSON response parsing to get user Facebook profile.

- (void)proccessMyFBProfile:(NSString*)input
{
    SBJsonParser *jsonParser = [[SBJsonParser alloc] init];
    NSError *error = nil;
    NSDictionary *result = [jsonParser objectWithString:input error:&error];
    [jsonParser release], jsonParser = nil;
    if (!error)
    {
        NSString *fullName = [[result objectForKey:keyJSONContactFullName] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        if ([fullName length] > 0)
            [[OpenPeerUser sharedOpenPeerUser] setFullName:fullName];
        
        //Provider key for Facebook 
        NSNumber* providerKey = [NSNumber numberWithInt:HOPProvisioningAccountIdentityTypeFacebookID];
        if (providerKey)
            [[OpenPeerUser sharedOpenPeerUser] setProviderKey:providerKey];
        
        //User facebook id
        NSString* cotnactProviderId = [result objectForKey:keyJSONContactId];
        if ([cotnactProviderId length] > 0)
            [[OpenPeerUser sharedOpenPeerUser] setContactProviderId:cotnactProviderId];
    }
}

      //-----------------------------------------------------------------------
      // Does JSON response parsing to get the list of Facebook contacts

- (void)proccessFbFriends:(NSString*)input
{
    //Parse JSON to get the contacts
    SBJsonParser *jsonParser = [[SBJsonParser alloc] init];
    NSError *error = nil;
    NSArray *result = [jsonParser objectWithString:input error:&error];
    [jsonParser release], jsonParser = nil;
    
    if (!error)
    {
        //Provider key for Facebook 
        NSNumber* providerKey = [NSNumber numberWithInt:HOPProvisioningAccountIdentityTypeFacebookID];
        NSMutableDictionary* contacts = [[NSMutableDictionary alloc] init];
        
        for (NSDictionary* dict in result)
        {
            //Get contact facebook id
            NSString* providerContactId = [dict objectForKey:keyJSONContactId];
            
            if (providerContactId)
            {
                //Get contact fullname
                NSString *fullName = [[NSString stringWithFormat:@"%@", [dict objectForKey:@"fullName"] ] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
                if (fullName)
                {
                    //Get avatar url
                    NSString *avatarUrl = [dict objectForKey:keyJSONContactPictureURL];
                    
                    //HOP_TODO: Try to get contact for identites, or create it
                    Contact* contact = [[Contact alloc] initWithFullName:fullName profession:@"" avatarUrl:avatarUrl identityProvider:HOPProvisioningAccountIdentityTypeFacebookID identityContactId:providerContactId];
                    
                    [self.mContacts addObject:contact];
                    [contacts setObject:contact forKey:providerContactId];
                    [contact release];
                }
            }
        }
        [self.contactsDictionaryByProvider setObject:contacts forKey:providerKey];
        [contacts release];
    }
    
    [[[[OpenPeer sharedOpenPeer] mainViewController] contactsTableViewController] onContactsLoaded];
    
    [self contactsLookupQuery:self.mContacts];
    [[[[OpenPeer sharedOpenPeer] mainViewController] contactsTableViewController] onContactsPeerFilesLoadingStarted];
}

      //-----------------------------------------------------------------------
      // Send request to get the peer files for specified list of contacts

- (void)peerFileLookupQuery:(NSArray *)contacts
{
    NSMutableArray* hopContacts = [[NSMutableArray alloc] init];
    
    //Create list of hopContact objects
    for (Contact* contact in self.mContacts)
    {
        if (contact.hopContact)
            [hopContacts addObject:contact.hopContact];
    }
    
    //Ask for peer files for passed contacts
    [[HOPProvisioningAccount sharedProvisioningAccount] peerFileLookup:self contacts:hopContacts];
    
    [hopContacts release];
}

      //-----------------------------------------------------------------------
      // Retrieves contact for passed list of identities.
      // IN: identities NSArray List of identities.
      // RETURN Contact with specified identities.

- (Contact*) getContactForIdentities:(NSArray*) identities
{
    Contact* contact = nil;
    for (HOPIdentity* identity in identities)
    {
        contact = [[self.contactsDictionaryByProvider objectForKey:[NSNumber numberWithInt:identity.identityType]] objectForKey:identity.identityId];
        if (contact)
            break;
    }
    
    return contact;
}

      //-----------------------------------------------------------------------

- (void) onAccountIdentityLookupQueryComplete:(HOPProvisioningAccountIdentityLookupQuery*) query
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if([query isComplete] && [query didSucceed])
        {
            for(HOPContact* hopContact in [query getContacts])
            {
                Contact* contact = [self getContactForIdentities:[hopContact getIdentities]];
                contact.hopContact = hopContact;
            }
            
            [[[[OpenPeer sharedOpenPeer] mainViewController] contactsTableViewController] onContactsLoaded];
            
            [self peerFileLookupQuery:self.mContacts];
        }
    });
}

      //-----------------------------------------------------------------------

- (void) onAccountPeerFileLookupQueryComplete:(HOPProvisioningAccountPeerFileLookupQuery*) query
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [[[[OpenPeer sharedOpenPeer] mainViewController] contactsTableViewController] onContactsLoaded];
    });
}
*/
