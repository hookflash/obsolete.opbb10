#include "loginPane.h"
#include "rootPane.h"
#include "applicationui.h"
#include "userIdentity.h"
#include "session.h"

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

LoginPane::LoginPane(shared_ptr<Session> session, RootPane* rootPane, NavigationPane* navigationPane) :
      QObject(rootPane),
      mRootPane(rootPane),
      mPageHasLoaded(false)
{
  mLoginUIDelegate = boost::shared_ptr<LoginPaneLoginUIDelegate>(new LoginPaneLoginUIDelegate(this));

  mIdentity = session->GetIdentity();
  mIdentity->SetLoginUIDelegate(mLoginUIDelegate);
  std::string uri = session->GetIdentityURI();
  std::string url = mIdentity->GetRedirectAfterLoginCompleteURL();
  mIdentity->BeginLogin(uri);

  QmlDocument* qml = QmlDocument::create("asset:///weblogin.qml").parent(this);
  qml->setContextProperty("paneParent", this);

  // create root object for the UI
  mPage = qml->createRootObject<Page>();

  navigationPane->push(mPage);

  bb::cascades::Container* containerObj = mPage->findChild<bb::cascades::Container*>("containerObj");
  mWebView = containerObj->findChild<WebView*>("webView");
  mWebView->storage()->clearCache();
}

void LoginPane::OnLoginClick()
{
  qDebug() << "***************** LoginPane::OnLoginClick";
  return;
}

bool LoginPane::OnNavigationRequested(QUrl url)
{
  QString urlFull = url.toString();
  qDebug() << "******** OnNavigationRequested = " << urlFull;

  QString methodValue = url.queryItemValue("method");
  if(methodValue.indexOf("notifyClient", 0) == 0) {
    std::string fullDataAsStdString = methodValue.toUtf8().constData();
    std::string dataAsStdString = fullDataAsStdString.substr(strlen("notifyClient;data="));

    mIdentity->OnNotifyClient(dataAsStdString);
    return true; // Cancel
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
