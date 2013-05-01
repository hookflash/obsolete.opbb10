#include "session.h"
#include "userIdentity.h"
#include "account.h"
#include "contactsManager.h"


using namespace hookflash::blackberry;
using namespace hookflash::core;
using namespace boost;

namespace {
  const char* DEFAULT_IDENTITY_URI = "identity://hookflash.me/";
  const char* BLACKBERRY_DEVICE_ID = "99deaf90a4944e86b00ff519149bcabe";
  const char* USER_AGENT = "hookflash/1.0.0 alpha (Blackberry)";
  const char* OS = "QNX";
  const char* SYSTEM = "Blackberry 10";
  const char* PEER_CONTACT_SERVICE_DOMAIN = "unstable.hookflash.me";
  const char* CONTACTS_URL = "http://example-light.hookflash.me/fbconnections.html";
};

  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------

SessionPtr Session::CreateInstance(ApplicationUI* appUI)
{
  shared_ptr<Session> pThis(new Session(appUI));
  pThis->mThisWeak = pThis;
  pThis->Initialize();
  return pThis;
}

  //-------------------------------------------------------------------------

Session::Session(ApplicationUI* appUI) : mAppUI(appUI), mPeerContactServiceDomain(PEER_CONTACT_SERVICE_DOMAIN), mContactsURL(CONTACTS_URL)
{
}

//-------------------------------------------------------------------------
void Session::Initialize()
{
  mStack = IStack::singleton();
  mStack->setup(
                mThisWeak.lock(),
                mThisWeak.lock(),
                BLACKBERRY_DEVICE_ID,
                USER_AGENT,
                OS,
                SYSTEM
                );

  mContactsManager = ContactsManager::CreateInstance(mThisWeak.lock());
  mIdentity = UserIdentity::CreateInstance(mThisWeak.lock());
  mAccount  = Account::CreateInstance(mThisWeak.lock());
  mIdentityURI = DEFAULT_IDENTITY_URI;
}
