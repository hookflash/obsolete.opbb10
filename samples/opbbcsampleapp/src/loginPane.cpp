#include "loginPane.h"
#include "rootPane.h"
#include "applicationui.h"
#include "identity.h"
#include "session.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>
#include <QDebug>
#include <iostream>

using namespace bb::cascades;
using namespace hookflash::blackberry;
using namespace boost;

LoginPane::LoginPane(shared_ptr<Session> session, RootPane* rootPane, NavigationPane* navigationPane) : QObject(rootPane), mRootPane(rootPane)
{
  mIdentity = session->GetIdentity();

  QmlDocument* qml = QmlDocument::create("asset:///weblogin.qml").parent(this);
  qml->setContextProperty("paneParent", this);

  // create root object for the UI
  Page* root = qml->createRootObject<Page>();

  navigationPane->push(root);
}

void LoginPane::OnLoginClick()
{
  qDebug() << "***************** LoginPane::OnLoginClick";
  return;
}

void LoginPane::OnLoadingChanged(int status, QUrl url)
{
  QString urlFull = url.toString();
  qDebug() << "******** status=" << status << " url=" << urlFull;
//  if(status == )
}

void LoginPane::TestCallback()
{
  qDebug() << "***************** LoginPane::TestCallback";
  return;
}
