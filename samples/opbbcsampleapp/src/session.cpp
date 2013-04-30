#include "session.h"
#include "userIdentity.h"
#include "account.h"


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

shared_ptr<Session> Session::CreateInstance()
{
  shared_ptr<Session> instance(new Session());
  instance->mWeakThis = instance;
  instance->Initialize();
  return instance;
}

  //-------------------------------------------------------------------------

Session::Session() : mPeerContactServiceDomain(PEER_CONTACT_SERVICE_DOMAIN), mContactsURL(CONTACTS_URL)
{
}

  //-------------------------------------------------------------------------

void Session::Initialize()
{
  mStackDelegate = boost::shared_ptr<SessionStackDelegate>(new SessionStackDelegate(mWeakThis.lock()));
  mMediaEngineDelegate = boost::shared_ptr<SessionMediaEngineDelegate>(new SessionMediaEngineDelegate(mWeakThis.lock()));

  mStack = IStack::singleton();
  mStack->setup(mStackDelegate,
                mMediaEngineDelegate,
                BLACKBERRY_DEVICE_ID,
                USER_AGENT,
                OS,
                SYSTEM);

  mIdentity = UserIdentity::CreateInstance(mWeakThis.lock());
  mAccount  = Account::CreateInstance(mWeakThis.lock());
  mIdentityURI = DEFAULT_IDENTITY_URI;
}
