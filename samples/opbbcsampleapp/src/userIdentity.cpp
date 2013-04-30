#include "userIdentity.h"
#include "session.h"
#include "account.h"
#include <QDebug>
#include <QRegExp>
#include <zsLib/XML.h>
#include <hookflash/core/IHelper.h>

using namespace hookflash::blackberry;
using namespace hookflash::core;
using namespace boost;

namespace {
  const char* REDIRECT_URL = "http://bogusredirecturl.hookflash.com/";
};

  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------

shared_ptr<UserIdentity> UserIdentity::CreateInstance(boost::shared_ptr<Session> session)
{
  shared_ptr<UserIdentity> instance(new UserIdentity(session));
  instance->mWeakThis = instance;
  return instance;
}

  //-------------------------------------------------------------------------

UserIdentity::UserIdentity(boost::shared_ptr<Session> session) : mSession(session), mRedirectAfterLoginCompleteURL(REDIRECT_URL)
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

  mOpIdentity = IIdentity::login(
      mDelegate,
      mRedirectAfterLoginCompleteURL.c_str(),
      "identity://facebook.com/",
      "unstable.hookflash.me");
  if(mOpIdentity) {
    qDebug() << "******** valid identity";
  }
}

void UserIdentity::OnNotifyClient(const std::string& data)
{
  mOpIdentity->handleMessageFromInnerBrowserWindowFrame(hookflash::core::IHelper::createFromString(data));
}

void UserIdentity::OnAccountStateReady()
{
  mLoginUIDelegate->NavigateTo(mSession->GetContactsURL());
}

bool UserIdentity::OnWebBrowserPageNavigation(const std::string& url)
{
  if(url == mRedirectAfterLoginCompleteURL) {
    mOpIdentity->notifyLoginCompleteBrowserWindowRedirection();
    return true;
  }
  return false;
}

void UserIdentity::OnWaitingToMakeBrowserWindowVisible()
{
  // TODO: mLoginUIDelegate->MakeBrowserWindowVisible();
  mOpIdentity->notifyBrowserWindowVisible();
}

void UserIdentity::OnWaitingToLoadBrowserWindow()
{
  std::string js = "initInnerFrame('";
  js += mOpIdentity->getIdentityLoginURL();
  js += "')";
  mLoginUIDelegate->CallJavaScript(js);
}

void UserIdentity::OnWaitingAssociation()
{
  mSession->GetAccount()->Login();
}

void UserIdentity::OnMessageForInnerBrowserWindowFrame(const std::string& message)
{
  std::string js = "sendNotifyBundleToInnerFrame('";
  js += message;
  js += "')";
  mLoginUIDelegate->CallJavaScript(js);
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

  if(state == IIdentity::IdentityState_WaitingToLoadBrowserWindow) {
    mParentIdentity.lock()->OnWaitingToLoadBrowserWindow();
  }
  else if(state == IIdentity::IdentityState_WaitingToMakeBrowserWindowVisible) {
    mParentIdentity.lock()->OnWaitingToMakeBrowserWindowVisible();
  }
  else if(state == IIdentity::IdentityState_WaitingAssociation) {
    mParentIdentity.lock()->OnWaitingAssociation();
  }
}

  //-------------------------------------------------------------------------

void UserIdentityDelegate::onIdentityPendingMessageForInnerBrowserWindowFrame(hookflash::core::IIdentityPtr identity)
{
  std::string message =  hookflash::core::IHelper::convertToString(identity->getNextMessageForInnerBrowerWindowFrame());
  mParentIdentity.lock()->OnMessageForInnerBrowserWindowFrame(message);
  qDebug() << "********** onIdentityPendingMessageForInnerBrowserWindowFrame = " << message.c_str();
}


