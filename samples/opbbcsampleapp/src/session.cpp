#include "session.h"
#include "identity.h"

using namespace hookflash::blackberry;
using namespace hookflash::core;
using namespace boost;

  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------

Session::Session()
{
}

shared_ptr<Session> Session::CreateInstance()
{
  shared_ptr<Session> instance(new Session());
  instance->mWeakThis = instance;
  instance->Initialize();
  return instance;
}

void Session::Initialize()
{
  mIdentity = shared_ptr<Identity>(new Identity());
}
