#include "rootPane.hpp"
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
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

  std::cerr << L"***************** RootPane" << std::endl;
}

void RootPane::OnLoginClick(QObject* pageObject)
{
  const QMetaObject* meta = pageObject->metaObject();
  const char* name = meta->className();

  NavigationPane* page = qobject_cast<NavigationPane*>(pageObject);
  if(page != NULL)
  {
    std::cerr << L"***************** RootPane::OnLoginClick YES" << std::endl;
  }
  std::cerr << L"***************** RootPane::OnLoginClick" << std::endl;
  return;
}

