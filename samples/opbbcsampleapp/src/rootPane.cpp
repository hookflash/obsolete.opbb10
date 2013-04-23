#include "rootPane.h"
#include "loginPane.h"
#include "applicationui.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Control>
#include <QDebug>
#include <iostream>

//#include <hookflash/core/internal/core_MediaEngine.h>
//#include <hookflash/core/test/TestMediaEngine.h>

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
    mLoginPane = new LoginPane(mAppUI->GetSession(), this, navigationPane);
    qDebug() << "***************** RootPane::OnLoginClick YES";
  }
  qDebug() << "***************** RootPane::OnLoginClick";
  return;
}

void RootPane::OnOnLoadingChanged(int status, QString url)
{
  qDebug() << "***************** RootPane::OnLoginClick";
}


void RootPane::OnMediaTestButton1Click()
{
  qDebug() << "***************** RootPane::OnMediaTestButton1Click";
/*
  hookflash::core::IMediaEngineDelegatePtr mediaEngineDelegatePtr;
  hookflash::core::test::TestMediaEngineFactoryPtr overrideFactory(new hookflash::core::test::TestMediaEngineFactory);
  hookflash::core::internal::Factory::override(overrideFactory);
  hookflash::core::internal::IMediaEngineForStack::setup(mediaEngineDelegatePtr);

  hookflash::core::internal::MediaEnginePtr mediaEngineInternal = hookflash::core::internal::IMediaEngineForCallTransport::singleton();
  hookflash::core::test::TestMediaEnginePtr testMediaEngineInternal = boost::dynamic_pointer_cast<hookflash::core::test::TestMediaEngine>(mediaEngineInternal);
  hookflash::core::IMediaEnginePtr mediaEngine = hookflash::core::IMediaEngine::singleton();

  mediaEngine->setEcEnabled(true);
  mediaEngine->setAgcEnabled(true);
  mediaEngine->setNsEnabled(false);
  mediaEngine->setMuteEnabled(false);
  mediaEngine->setLoudspeakerEnabled(false);
  mediaEngine->setContinuousVideoCapture(true);
  mediaEngine->setDefaultVideoOrientation(hookflash::core::IMediaEngine::VideoOrientation_Portrait);
  mediaEngine->setRecordVideoOrientation(hookflash::core::IMediaEngine::VideoOrientation_LandscapeRight);
  mediaEngine->setFaceDetection(false);

  mediaEngine->setCaptureRenderView(NULL);
  mediaEngine->setChannelRenderView(NULL);

  mediaEngine->startVideoCapture();

  testMediaEngineInternal->setReceiverAddress("127.0.0.1");

  mediaEngineInternal->forCallTransport().startVoice();
  mediaEngineInternal->forCallTransport().startVideoChannel();
*/
}

void RootPane::OnMediaTestButton2Click()
{
  qDebug() << "***************** RootPane::OnMediaTestButton2Click";
/*
  hookflash::core::internal::MediaEnginePtr mediaEngineInternal = hookflash::core::internal::IMediaEngineForCallTransport::singleton();
  hookflash::core::test::TestMediaEnginePtr testMediaEngineInternal = boost::dynamic_pointer_cast<hookflash::core::test::TestMediaEngine>(mediaEngineInternal);
  hookflash::core::IMediaEnginePtr mediaEngine = hookflash::core::IMediaEngine::singleton();

  mediaEngineInternal->forCallTransport().stopVoice();
  mediaEngineInternal->forCallTransport().stopVideoChannel();

  mediaEngine->stopVideoCapture();
*/
}
