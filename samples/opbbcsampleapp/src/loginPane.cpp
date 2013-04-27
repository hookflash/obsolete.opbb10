#include "loginPane.h"
#include "rootPane.h"
#include "applicationui.h"
#include "userIdentity.h"
#include "session.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>
#include <bb/cascades/Container>
#include <QDebug>
#include <iostream>

using namespace bb::cascades;
using namespace hookflash::blackberry;
using namespace boost;

LoginPane::LoginPane(shared_ptr<Session> session, RootPane* rootPane, NavigationPane* navigationPane) : QObject(rootPane), mRootPane(rootPane)
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
}

void LoginPane::OnLoginClick()
{
  qDebug() << "***************** LoginPane::OnLoginClick";
  return;
}

bool LoginPane::OnNavigationRequested(QUrl url)
{
  QString urlFull = url.toString();
  std::string urlAsStdString = urlFull.toUtf8().constData();
  mIdentity->OnWebBrowserPageLoaded(urlAsStdString);
  qDebug() << "******** OnNavigationRequested = " << urlAsStdString.c_str();
  return false;

}

void LoginPane::OnLoadingChanged(int status, QUrl url)
{
  QString urlFull = url.toString();
  std::string urlAsStdString = urlFull.toUtf8().constData();
  qDebug() << "******** OnLoadChanged = " << urlAsStdString.c_str();
  mIdentity->OnWebBrowserPageLoaded(urlAsStdString);

}

void LoginPane::TestCallback()
{
  qDebug() << "***************** LoginPane::TestCallback";
  return;
}

void LoginPane::CallJavaScript(const std::string& js)
{
  bb::cascades::Container* containerObj = mPage->findChild<bb::cascades::Container*>("containerObj");
  mWebView = containerObj->findChild<WebView*>("webView");

  qDebug() << "********* LoginPane::CallJavaScript = " << js.c_str();

  QString jsq(js.c_str());
  bool status = mWebView->evaluateJavaScript(jsq);
}

