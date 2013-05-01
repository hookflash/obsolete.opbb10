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

#include "contactsManager.h"
#include "contact.h"

#include <hookflash/stack/IServiceIdentity.h>

#include <zsLib/XML.h>
#include <zsLib/Stringize.h>
#include <zsLib/Log.h>
#include <zsLib/helpers.h>

namespace hookflash { namespace blackberry { ZS_DECLARE_SUBSYSTEM(hookflash_blackberry) } }

using namespace hookflash::blackberry;

using zsLib::String;
using zsLib::Stringize;
using zsLib::XML::Document;
using zsLib::XML::DocumentPtr;
using zsLib::XML::Element;
using zsLib::XML::ElementPtr;
using hookflash::core::IContactPtr;
using hookflash::core::IdentityLookupInfo;
using hookflash::core::IdentityLookupInfoList;
using hookflash::stack::IServiceIdentity;

ContactsManagerPtr ContactsManager::CreateInstance(SessionPtr session)
{
  ContactsManagerPtr pThis(new ContactsManager(session));
  pThis->mThisWeak = pThis;
  return pThis;
}

ContactsManager::ContactsManager(SessionPtr session) :
  mID(zsLib::createPUID()),
  mSession(session)
{
  ZS_LOG_DEBUG(log("created"))
}

ContactsManager::~ContactsManager()
{
  mThisWeak.reset();
  ZS_LOG_DEBUG(log("destroyed"))
}

void ContactsManager::LoadContacts()
{
}

void ContactsManager::AddContactsFromJSON(const std::string& json)
{
  ZS_LOG_DEBUG(log("adding contacts from JSON") + ", json=" + json)
  DocumentPtr document = Document::createFromAutoDetect(json.c_str());

  if (!document) return;

  ElementPtr unknownEl = document->getFirstChildElement();
  while (unknownEl) {
    ElementPtr idEl = unknownEl->findFirstChildElement("id");
    ElementPtr fullNameEl = unknownEl->findFirstChildElement("fullName");
    ElementPtr pictureURLEl = unknownEl->findFirstChildElement("pictureUrl");

    String id;
    String fullName;
    String pictureURL;

    if (idEl) {
      id = idEl->getTextDecoded();
    }
    if (fullNameEl) {
      fullName = fullNameEl->getTextDecoded();
    }
    if (pictureURLEl) {
      pictureURL = pictureURLEl->getTextDecoded();
    }

    if ((id.hasData()) ||
        (fullName.hasData()) ||
        (pictureURL.hasData())) {
      AddContact("facebook.com", fullName, id, pictureURL);
    }

    unknownEl = unknownEl->getNextSiblingElement();
  }
  ZS_LOG_DEBUG(log("adding contacts from JSON complete"))
}

void ContactsManager::AddContact(
  const char *identityDomain,
  const char* fullName,
  const char* id,
  const char* pictureURL
)
{

  std::string identityURI = IServiceIdentity::joinURI(identityDomain, id);

  ZS_LOG_DEBUG(log("adding contact") + ", fullname=" + fullName + ", id=" + id + ", picture URL=" + pictureURL + ", identity URI=" + identityURI)

  ContactPtr newContact(new Contact(fullName, id, pictureURL, identityURI));
  mContacts.push_back(newContact);

  mContactsByIdentity[identityURI] = newContact;
}

bool ContactsManager::prepareIdentityURIListForIdentityLookup(IdentityURIList &outList)
{
  ZS_LOG_DEBUG(log("preparing Identity URI list"))

  for (ContactVector::iterator iter = mContacts.begin(); iter != mContacts.end(); ++iter) {
    ContactPtr &contact = (*iter);
    IContactPtr coreContact = contact->GetContact();

    std::string identityURI = contact->GetIdentityURI();
    if (identityURI.length() < 1) {
      // three is no ID for this contact
      continue;
    }

    outList.push_back(identityURI);
  }

  ZS_LOG_DEBUG(log("preparing Identity URI list complete") + ", total=" + Stringize<IdentityURIList::size_type>(outList.size()).string())

  return (outList.size() > 0);
}

void ContactsManager::handleIdentityLookupResult(IdentityLookupInfoListPtr result)
{
  if (!result) {
    ZS_LOG_ERROR(Debug, log("handle identity look result is NULL result"))
    return;
  }

  ZS_LOG_DEBUG(log("handle identity lookup result") + ", total=" + Stringize<IdentityLookupInfoList::size_type>(result->size()).string())

  for (IdentityLookupInfoList::iterator iter = result->begin(); iter != result->end(); ++iter)
  {
    IdentityLookupInfo &info = (*iter);

    String domain;
    String id;

    ZS_LOG_DEBUG(log("contact found") + ", identity URI=" + info.mIdentityURI)

    if (!info.mContact) {
      // no core "contact" object was found, thus skipping since there's nothing to do
      ZS_LOG_WARNING(Debug, log("identity not associated to peer contact") + ", identity URI=" + info.mIdentityURI)
      continue;
    }

    ContactMap::iterator found = mContactsByIdentity.find(info.mIdentityURI);
    if (found == mContactsByIdentity.end()) {
      // this contact was not found
      ZS_LOG_ERROR(Debug, log("identity was not found in contact manager") + ", identity URI=" + info.mIdentityURI)
      continue;
    }
    ContactPtr &contact = (*found).second;
    contact->SetContact(info.mContact);
  }

}

bool ContactsManager::prepareContactListForContactPeerFilePublicLookup(ContactList &outList)
{
  for (ContactVector::iterator iter = mContacts.begin(); iter != mContacts.end(); ++iter) {
    ContactPtr &contact = (*iter);
    IContactPtr coreContact = contact->GetContact();
    ZS_LOG_DEBUG(log("preparing") + ", fullname=" + contact->GetFullName() + ", id=" + contact->GetId() + ", picture URL=" + contact->GetPictureURL() + ", identity URI=" + contact->GetIdentityURI())
    if (!coreContact) {
      // there is no core contact thus we do not need to perform a lookup on this contact
      ZS_LOG_DEBUG("contact does not have core contact")
      continue;
    }
    if (coreContact->hasPeerFilePublic()) {
      // there is no need to lookup this contact since it has a core contact already
      ZS_LOG_DEBUG("contact already has peer file public")
      continue;
    }
    outList.push_back(coreContact);
  }

  return (outList.size() > 0);
}

String ContactsManager::log(const char *message) const
{
  return String("ContactsManager [") + Stringize<typeof(mID)>(mID).string() + "] " + message;
}
