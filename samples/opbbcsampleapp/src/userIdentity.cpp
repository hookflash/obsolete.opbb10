#include "userIdentity.h"
#include <QDebug>
#include <QRegExp>

using namespace hookflash::blackberry;
using namespace hookflash::core;
using namespace boost;

namespace {
  const char* REDIRECT_URL = "http://bogusredirecturl.hookflash.com";
};

  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------

shared_ptr<UserIdentity> UserIdentity::CreateInstance()
{
  shared_ptr<UserIdentity> instance(new UserIdentity());
  instance->mWeakThis = instance;
  return instance;
}

  //-------------------------------------------------------------------------

UserIdentity::UserIdentity() : mRedirectAfterLoginCompleteURL(REDIRECT_URL)
{
}

  //-------------------------------------------------------------------------

UserIdentity::~UserIdentity()
{
}

  //-------------------------------------------------------------------------

void UserIdentity::BeginLogin(const std::string& identityURI)
{
  mDelegate = shared_ptr<UserIdentityDelegate>(new UserIdentityDelegate(mWeakThis.lock()));

//  mOpIdentity = IIdentity::login(identityDelegate, mRedirectAfterLoginCompleteURL.c_str(), identityURI.c_str(), "hookflash.me");
  mOpIdentity = IIdentity::login(mDelegate, mRedirectAfterLoginCompleteURL.c_str(), "identity://unstable.hookflash.me/lawrence", "unstable.hookflash.me");
  if(mOpIdentity) {
    qDebug() << "******** valid identity";
  }
}

  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------

UserIdentityDelegate::UserIdentityDelegate(shared_ptr<UserIdentity> parentIdentity) : mParentIdentity(parentIdentity)
{

}

  //-------------------------------------------------------------------------

UserIdentityDelegate::~UserIdentityDelegate()
{
}

  //-------------------------------------------------------------------------

void UserIdentityDelegate::onIdentityStateChanged(hookflash::core::IIdentityPtr identity, IdentityStates state)
{
  qDebug() << "********** onIdentityStateChanged = " << IIdentity::toString(state);
  qDebug() << "********** URL = " << identity->getIdentityLoginURL();
}

  //-------------------------------------------------------------------------

void UserIdentityDelegate::onIdentityPendingMessageForInnerBrowserWindowFrame(hookflash::core::IIdentityPtr identity)
{
  ElementPtr element = identity->getNextMessageForInnerBrowerWindowFrame();
  qDebug() << "********** onIdentityPendingMessageForInnerBrowserWindowFrame";
}


