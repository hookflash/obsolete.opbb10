/*
 
 Copyright (c) 2013, SMB Phone Inc.
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

#ifndef _HF_CONTACTS_MANAGER_
#define _HF_CONTACTS_MANAGER_

#include "types.h"

#include <hookflash/core/IIdentityLookup.h>

#include <vector>
#include <map>

namespace hookflash {
  namespace blackberry {

    class ContactsManager {
    public:
      typedef zsLib::PUID PUID;
      typedef zsLib::String String;
      typedef hookflash::core::IdentityLookupInfoListPtr IdentityLookupInfoListPtr;
      typedef hookflash::core::ContactList ContactList;
      typedef hookflash::core::IdentityURIList IdentityURIList;

      typedef std::vector<ContactPtr> ContactVector;
      typedef std::string IdentityURI;
      typedef std::map<IdentityURI, ContactPtr> ContactMap;

    protected:
      ContactsManager(SessionPtr session);

    public:
      static ContactsManagerPtr CreateInstance(SessionPtr session);

      ~ContactsManager();

      ContactPtr FindContactByIdentityURI(const char *identityURI) const;
      ContactPtr FindContactBy(hookflash::core::IContactPtr contact) const;

      void LoadContacts();

      // STEP 1:  You must fetch contacts from Facebook and pass the json result into this method to parse all the contacts
      void AddContactsFromJSON(const std::string& json);

      void AddContact(
        const char *identityDomain,
        const char* fullName,
        const char* id,
        const char* pictureUrl
      );

      // STEP 2:  You must use the result of prepareIdentityURIListForIdentityLookup as a parameter to IIdentityLookup
      // PURPOSE: prepares the identity URI list for passing into IIdentityLookup object
      // RETURNS: true if there are contacts that need to be looked up, otherwise false
      // NOTE:    This causes the "peer URI" for each identity to become known (but does not load the public peer file of the cotnact)
      bool prepareIdentityURIListForIdentityLookup(IdentityURIList &outList);

      // STEP 3:  The result "IIdentityLookup::getIdentities()" must be passed into this method
      // PURPOSE: handles the result as returned from "IIdentityLookup"
      // NOTE:    This associates the Contact object to the core IContact object
      void handleIdentityLookupResult(IdentityLookupInfoListPtr result);

      // STEP 4:  Now that you know all the contact objects, you must look up their public peer files otherwise you cannot contact these users
      // PURPOSE: obtains the list of contacts which need to have their public peer file fetched
      // RETURNS: true if there are public peer files to lookup, otherwise false
      // NOTE:    there is nothing to call after completion of the lookup because the IContact object populates itself with the public peer file
      bool prepareContactListForContactPeerFilePublicLookup(ContactList &outList);

      const ContactVector& GetContactVector() { return mContacts; }

    protected:
      String log(const char *message) const;

    private:

      PUID mID;
      ContactsManagerWeakPtr mThisWeak;
      SessionPtr mSession;

      ContactVector mContacts;
      ContactMap mContactsByIdentity;

    };
  }
}

#endif // _HF_CONTACTS_MANAGER_
