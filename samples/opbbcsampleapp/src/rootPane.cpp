#include "rootPane.h"
#include "loginPane.h"
#include "applicationui.h"
#include "session.h"
#include "account.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Control>
#include <bb/cascades/Window>
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/ListView>
#include <bb/data/JsonDataAccess>
#include <QDebug>
#include <iostream>

#include <hookflash/core/internal/core_MediaEngine.h>
#include <hookflash/core/test/TestMediaEngine.h>

using namespace bb::cascades;
using namespace bb::data;
using namespace hookflash::blackberry;

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
RootPane::RootPane(ApplicationUI* appUI) : QObject(appUI), mAppUI(appUI), mQml(NULL), mCallWindowIsOpen(false)
{

  // create scene document from main.qml asset
  // set parent to created document to ensure it exists for the whole application lifetime
  mQml = QmlDocument::create("asset:///main.qml").parent(this);
  mQml->setContextProperty("paneParent", this);

  // create root object for the UI
  AbstractPane* root = mQml->createRootObject<AbstractPane>();


  // set created root object as a scene
  mAppUI->GetApplication()->setScene(root);


  mContactsListView = root->findChild<ListView*>("contactList");
  if(mContactsListView != NULL) {
    qDebug() << "SA[RootPane::RootPane] List view not found";
  }

  mForeignWindow = root->findChild<ForeignWindowControl*>("foreignWindow");
  if(mForeignWindow == NULL) {
    qDebug() << "SA[RootPane::RootPane] Foreign window not found";
    return;
  }

  LayoutUpdateHandler* handler = LayoutUpdateHandler::create(mForeignWindow).onLayoutFrameChanged(
    this,
    SLOT(onLayoutFrameChanged(const QRectF &)));
  mVideoWindowSize = handler->layoutFrame();


  std::string peerFile = mAppUI->GetSession()->GetAccount()->ReadPrivatePeerFile();
  std::string secret = mAppUI->GetSession()->GetAccount()->ReadPrivatePeerSecretFile();

  if(peerFile.size() > 0 && secret.size() > 0) {
    bool success = mAppUI->GetSession()->GetAccount()->Relogin(peerFile, secret);
    if(success) {
      // TODO: Don't show login screen;
    }
  }

//  ProcessFbFriends(QString(""));
}

//-----------------------------------------------------------------
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

//-----------------------------------------------------------------
void RootPane::OnOnLoadingChanged(int status, QString url)
{
  qDebug() << "***************** RootPane::OnLoginClick";
}


//-----------------------------------------------------------------
void RootPane::OnCallWindowOpened(QObject* callPageObj)
{
  mCallWindowIsOpen = true;
  if(!mVideoRenderer && !mVideoWindowSize.isNull()) {
    CreateVideoRenderer();
  }
}

//-----------------------------------------------------------------
void RootPane::OnMediaTestButton1Click()
{
  qDebug() << "***************** RootPane::OnMediaTestButton1Click";

  hookflash::core::IMediaEngineDelegatePtr mediaEngineDelegatePtr;
  hookflash::core::test::TestMediaEngineFactoryPtr overrideFactory(new hookflash::core::test::TestMediaEngineFactory);
  hookflash::core::internal::Factory::override(overrideFactory);
  hookflash::core::internal::IMediaEngineForStack::setup(mediaEngineDelegatePtr);

  hookflash::core::internal::MediaEnginePtr mediaEngineInternal = hookflash::core::internal::IMediaEngineForCallTransport::singleton();
  hookflash::core::test::TestMediaEnginePtr testMediaEngineInternal = boost::dynamic_pointer_cast<hookflash::core::test::TestMediaEngine>(mediaEngineInternal);
  hookflash::core::IMediaEnginePtr mediaEngine = hookflash::core::IMediaEngine::singleton();

//  mediaEngine->setEcEnabled(true);
//  mediaEngine->setAgcEnabled(true);
//  mediaEngine->setNsEnabled(false);
//  mediaEngine->setMuteEnabled(false);
//  mediaEngine->setLoudspeakerEnabled(false);
  mediaEngine->setContinuousVideoCapture(true);
//  mediaEngine->setDefaultVideoOrientation(hookflash::core::IMediaEngine::VideoOrientation_Portrait);
//  mediaEngine->setRecordVideoOrientation(hookflash::core::IMediaEngine::VideoOrientation_LandscapeRight);
//  mediaEngine->setFaceDetection(false);

//  mediaEngine->setCaptureRenderView(mVideoRenderer.get());
  mediaEngine->setChannelRenderView(mVideoRenderer.get());

  mediaEngine->startVideoCapture();

  testMediaEngineInternal->setReceiverAddress("127.0.0.1");

//  mediaEngineInternal->forCallTransport().startVoice();
  mediaEngineInternal->forCallTransport().startVideoChannel();

}

//-----------------------------------------------------------------
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

//-----------------------------------------------------------------
void RootPane::ProcessFbFriends(const QString& data)
{
  QString json = "[{\"id\": \"100003985703380\",\"fullName\": \"Boris Ikodinovic\",\"pictureUrl\": \"./images/contact.png\"},{\"id\": \"100004075097369\",\"fullName\": \"Nenad Nastasic\",\"pictureUrl\":\"./images/contact.png\"},{\"id\": \"100004281043574\",\"fullName\": \"Sergio Trifunovic\",\"pictureUrl\": \"./images/contact.png\"}]";

  // Create the data model, specifying sorting keys of "firstName" and "lastName"
  mContactModel = new GroupDataModel(QStringList() << "fullName");

  // Create a JsonDataAccess object and load the .json file. The
  // QDir::currentPath() function returns the current working
  // directory for the app.
  JsonDataAccess jda;
  QVariant list = jda.loadFromBuffer(json);

  // Insert the data into the data model. Because the root of the .json file
  // is an array, a QVariant(QVariantList) is returned from load(). You can
  // provide a QVariantList to a data model directly by using insertList().
  mContactModel->insertList(list.value<QVariantList>());
//  QVariantMap itemMap;
//  itemMap["id"] = "123456";
//  itemMap["fullName"] = "Erik Langerway";
//  itemMap["pictureUrl"] = "../images/contact.png";
//  mContactModel->insert(itemMap);
  int n = mContactModel->size();

  // Set the data model for the list view
//  mContactsListView->resetDataModel();
//  model->
  mContactsListView->setDataModel(mContactModel);
}

//-----------------------------------------------------------------
void RootPane::onLayoutFrameChanged(const QRectF &layoutFrame) {
  if(!mVideoRenderer) {
    mVideoWindowSize = layoutFrame;
    CreateVideoRenderer();
    //OnMediaTestButton1Click();
  }
}

//-----------------------------------------------------------------
void RootPane::CreateVideoRenderer() {
  QString groupIdQ = mForeignWindow->windowGroup();
  const char* groupId = groupIdQ.toAscii();

  QString windowIdQ = mForeignWindow->windowId();
  const char* windowId = windowIdQ.toAscii();

  mVideoRenderer = boost::shared_ptr<webrtc::BlackberryWindowWrapper>(
		  new webrtc::BlackberryWindowWrapper(windowId,
				  	  	  	  	  	  	  	   groupId,
											   mVideoWindowSize.width(),
											   mVideoWindowSize.height()));
}
