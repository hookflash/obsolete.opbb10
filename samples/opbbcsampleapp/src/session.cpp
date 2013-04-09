#include "session.h"
#include "userIdentity.h"

using namespace hookflash::blackberry;
using namespace hookflash::core;
using namespace boost;

namespace {
  const char* DEFAULT_IDENTITY_URI = "identity://hookflash.me";
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

Session::Session()
{
}

  //-------------------------------------------------------------------------

void Session::Initialize()
{
  mIdentity = UserIdentity::CreateInstance();
  mIdentityURI = DEFAULT_IDENTITY_URI;
}
