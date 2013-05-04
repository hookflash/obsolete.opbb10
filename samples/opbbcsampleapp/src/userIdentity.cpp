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

UserIdentityPtr UserIdentity::CreateInstance(SessionPtr session)
{
  UserIdentityPtr pThis(new UserIdentity(session));
  pThis->mThisWeak = pThis;
  return pThis;
}

  //-------------------------------------------------------------------------

UserIdentity::UserIdentity(SessionPtr session) : mSession(session), mRedirectAfterLoginCompleteURL(REDIRECT_URL)
{
}

  //-------------------------------------------------------------------------

UserIdentity::~UserIdentity()
{
}

  //-------------------------------------------------------------------------

void UserIdentity::BeginLogin(const std::string& identityURI)
{
  mOpIdentity = IIdentity::login(
      mThisWeak.lock(),
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
  mLoginUIDelegate->LoginMakeBrowserWindowVisible();
  mOpIdentity->notifyBrowserWindowVisible();
}

void UserIdentity::OnWaitingLoginCompleteBrowserRedirection()
{
  mLoginUIDelegate->LoginHideBrowserAfterLogin();
}

void UserIdentity::OnWaitingToLoadBrowserWindow()
{
  std::string js = "initInnerFrame('";
  js += mOpIdentity->getIdentityLoginURL();
  js += "')";
  mLoginUIDelegate->LoginCallJavaScript(js);
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
  mLoginUIDelegate->LoginCallJavaScript(js);
}

void UserIdentity::onIdentityStateChanged(hookflash::core::IIdentityPtr identity, IdentityStates state)
{
  qDebug() << "********** onIdentityStateChanged = " << IIdentity::toString(state);

  if(state == IIdentity::IdentityState_WaitingToLoadBrowserWindow) {
    OnWaitingToLoadBrowserWindow();
  }
  else if(state == IIdentity::IdentityState_WaitingToMakeBrowserWindowVisible) {
    OnWaitingToMakeBrowserWindowVisible();
  }
  else if(state == IIdentity::IdentityState_WaitingLoginCompleteBrowserRedirection) {
    OnWaitingLoginCompleteBrowserRedirection();
  }
  else if(state == IIdentity::IdentityState_WaitingAssociation) {
    OnWaitingAssociation();
  }
}

void UserIdentity::onIdentityPendingMessageForInnerBrowserWindowFrame(hookflash::core::IIdentityPtr identity)
{
  std::string message =  hookflash::core::IHelper::convertToString(identity->getNextMessageForInnerBrowerWindowFrame());
  OnMessageForInnerBrowserWindowFrame(message);
  qDebug() << "********** onIdentityPendingMessageForInnerBrowserWindowFrame = " << message.c_str();
}
