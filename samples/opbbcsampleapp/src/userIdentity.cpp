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
  QRegExp reg("^identity:[a-zA-Z0-9\\-_]{0,61}:.*$");
  int ind = reg.indexIn(QString(identityURI.c_str()));
  qDebug() << "******* index = " << ind;
  mDelegate = shared_ptr<UserIdentityDelegate>(new UserIdentityDelegate(mWeakThis.lock()));
  shared_ptr<IIdentityDelegate> identityDelegate = mDelegate;
  const char* str = IIdentity::toString(IIdentity::IdentityState_Pending);
  qDebug() << "******* test = " << str;

  mOpIdentity = IIdentity::login(identityDelegate, mRedirectAfterLoginCompleteURL.c_str(), identityURI.c_str());
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
//  qDebug() << "********** onIdentityStateChanged = " << IIdentity::toString(state);
}

  //-------------------------------------------------------------------------

void UserIdentityDelegate::onIdentityPendingMessageForInnerBrowserWindowFrame(hookflash::core::IIdentityPtr identity)
{
}


