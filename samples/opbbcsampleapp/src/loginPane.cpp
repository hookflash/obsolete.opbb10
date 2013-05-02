#include "loginPane.h"
#include "rootPane.h"
#include "applicationui.h"
#include "userIdentity.h"
#include "session.h"
#include "account.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>
#include <bb/cascades/Container>
#include <bb/cascades/WebStorage>
#include <bb/cascades/WebLoadStatus>
#include <QDebug>
#include <iostream>

using namespace bb::cascades;
using namespace hookflash::blackberry;
using namespace boost;

LoginPane::LoginPane(shared_ptr<Session> session, RootPane* rootPane) :
      QObject(rootPane),
      mSession(session),
      mRootPane(rootPane),
      mPageHasLoaded(false)
{
  mThisDelegates = LoginPaneDelegatesPtr(new LoginPaneDelegates(this));

  mIdentity = mSession->GetIdentity();
  mIdentity->SetLoginUIDelegate(mThisDelegates);
  std::string uri = session->GetIdentityURI();
  std::string url = mIdentity->GetRedirectAfterLoginCompleteURL();
  mIdentity->BeginLogin(uri);

  QmlDocument* qml = QmlDocument::create("asset:///weblogin.qml").parent(this);
  qml->setContextProperty("paneParent", this);

  // create root object for the UI
  mPage = qml->createRootObject<Page>();

  bb::cascades::Container* containerObj = mPage->findChild<bb::cascades::Container*>("containerObj");
  mWebView = containerObj->findChild<WebView*>("webView");
  mWebView->storage()->clearCache();
}

LoginPane::~LoginPane()
{
  mThisDelegates->destroy();
  mThisDelegates.reset();
}


void LoginPane::OnLoginClick(NavigationPane* navigationPane)
{
  navigationPane->push(mPage);
}

bool LoginPane::OnNavigationRequested(QUrl url)
{
  QString urlFull = url.toString();
  qDebug() << "******** OnNavigationRequested = " << urlFull;

  QString methodValue = url.queryItemValue("method");
  if(methodValue.length() != 0) {
    if(methodValue.indexOf("notifyClient", 0) == 0) {
      std::string fullDataAsStdString = methodValue.toUtf8().constData();
      std::string dataAsStdString = fullDataAsStdString.substr(strlen("notifyClient;data="));

      mIdentity->OnNotifyClient(dataAsStdString);
      return true; // Cancel
    }
    else if(methodValue.indexOf("proccessMyFBProfile", 0) == 0) {
      std::string fullDataAsStdString = methodValue.toUtf8().constData();
      std::string dataAsStdString = fullDataAsStdString.substr(strlen("proccessMyFBProfile;data="));

      mSession->GetAccount()->ProcessMyFBProfile(dataAsStdString);
      return true; // Cancel
    }
    else if(methodValue.indexOf("proccessFbFriends", 0) == 0) {
      QString fullData = methodValue.mid(strlen("proccessFbFriends;data="));

      mRootPane->ProcessFbFriends(fullData);
      return true; // Cancel
    }
    else {
      qDebug() << "**** LoginPane::OnNavigationRequested did not process method type = " << methodValue;
      return true;
    }
  }
  else {
    QString urlFull = url.toString();
    std::string urlAsStdString = urlFull.toUtf8().constData();
    return mIdentity->OnWebBrowserPageNavigation(urlAsStdString);
  }
}

void LoginPane::OnLoadingChanged(int status, QUrl url)
{
  if(WebLoadStatus::Succeeded == status) {
    mPageHasLoaded = true;
    if(mJsToEvaluateWhenPageLoaded.size() > 0) {
      CallJavaScriptAfterPageLoad();
    }
  }
  QString urlFull = url.toString();
  std::string urlAsStdString = urlFull.toUtf8().constData();
  qDebug() << "******** OnLoadChanged = " << urlAsStdString.c_str();
//  mIdentity->OnWebBrowserPageLoaded(urlAsStdString);
}

void LoginPane::TestCallback()
{
  qDebug() << "***************** LoginPane::TestCallback";
  return;
}

void LoginPane::NavigateTo(const std::string& url)
{
  mWebView->setUrl(QUrl(url.c_str()));
}

void LoginPane::CallJavaScript(const std::string& js)
{
  mJsToEvaluateWhenPageLoaded = js;
  if(mPageHasLoaded) {
    CallJavaScriptAfterPageLoad();
  }
}

void LoginPane::CallJavaScriptAfterPageLoad()
{
  bb::cascades::Container* containerObj = mPage->findChild<bb::cascades::Container*>("containerObj");
  mWebView = containerObj->findChild<WebView*>("webView");
  mWebView->storage()->clearCache();

  qDebug() << "********* LoginPane::CallJavaScriptAfterPageLoad = " << mJsToEvaluateWhenPageLoaded.c_str();

  QString jsq(mJsToEvaluateWhenPageLoaded.c_str());
  mJsToEvaluateWhenPageLoaded.clear();
  bool status = mWebView->evaluateJavaScript(jsq);
}
