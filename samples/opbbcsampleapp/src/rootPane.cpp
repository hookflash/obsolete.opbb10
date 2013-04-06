#include "rootPane.hpp"
#include "loginPane.hpp"
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Control>
#include <QDebug>
#include <iostream>

using namespace bb::cascades;
using namespace hookflash::blackberry;

RootPane::RootPane(ApplicationUI* appUI) : QObject(appUI), mAppUI(appUI), mQml(NULL)
{

  // create scene document from main.qml asset
  // set parent to created document to ensure it exists for the whole application lifetime
  mQml = QmlDocument::create("asset:///main.qml").parent(this);
  mQml->setContextProperty("paneParent", this);

  // create root object for the UI
  AbstractPane* root = mQml->createRootObject<AbstractPane>();


  // set created root object as a scene
  mAppUI->GetApplication()->setScene(root);

  qDebug() << "***************** RootPane";
}

void RootPane::OnLoginClick(QObject* navigationPaneObj)
{
//  const QMetaObject* meta = pageObject->metaObject();
//  const char* name = meta->className();

  NavigationPane* navigationPane = qobject_cast<NavigationPane*>(navigationPaneObj);
//  Page* page = qobject_cast<Page*>(pageObject);
  if(navigationPane != NULL)
  {
    mLoginPane = new LoginPane(this, navigationPane);
    qDebug() << "***************** RootPane::OnLoginClick YES";
  }
  qDebug() << "***************** RootPane::OnLoginClick";
  return;
}

void RootPane::OnOnLoadingChanged(int status, QString url)
{
  qDebug() << "***************** RootPane::OnLoginClick";
}


