#include "loginPane.hpp"
#include "rootPane.hpp"
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>
#include <hookflash/core/IAccount.h>
#include <QDebug>
#include <iostream>

using namespace bb::cascades;
using namespace hookflash::blackberry;

LoginPane::LoginPane(RootPane* rootPane, NavigationPane* navigationPane) : QObject(rootPane), mRootPane(rootPane)
{
  qDebug() << "********* Hi ***********";
  QmlDocument* qml = QmlDocument::create("asset:///weblogin.qml").parent(this);
  qml->setContextProperty("paneParent", this);

  // create root object for the UI
  Page* root = qml->createRootObject<Page>();

  navigationPane->push(root);

  qDebug() << "***************** LoginPane";
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
