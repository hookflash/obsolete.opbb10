#include "rootPane.h"
#include "session.h"
#include "applicationui.h"
#include "session.h"
#include "account.h"
#include "contactsManager.h"
#include "contact.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Control>
#include <bb/cascades/Window>
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/ListView>
#include <bb/data/JsonDataAccess>
#include <bb/cascades/WebStorage>
#include <QDebug>
#include <QTimer>
#include <iostream>

#include <hookflash/core/internal/core_MediaEngine.h>
#include <hookflash/core/test/TestMediaEngine.h>

using namespace bb::cascades;
using namespace bb::data;
using namespace hookflash::blackberry;

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
RootPane::RootPane(ApplicationUI* appUI) :
    QObject(appUI),
    mAppUI(appUI),
    mQml(NULL),
    mCallWindowIsOpen(false),
    mLoginPageHasLoaded(false),
    mContactsPageHasLoaded(false)
{
  mThisDelegates = RootPaneDelegatesPtr(new RootPaneDelegates(this));

  // Create scene document from main.qml asset and set parent to created document to
  // ensure it exists for the whole application lifetime.
  mQml = QmlDocument::create("asset:///main.qml").parent(this);
  mQml->setContextProperty("cppParent", this);

  // Create root object for the UI.
  mRoot = mQml->createRootObject<AbstractPane>();

  // Set created root object as a scene.
  mAppUI->GetApplication()->setScene(mRoot);

  mContactsListView = mRoot->findChild<ListView*>("listViewContacts");
  mForeignWindow = mRoot->findChild<ForeignWindowControl*>("foreignWindow");

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

}

RootPane::~RootPane()
{
  // IMPORTANT: This prevents crashing on unfinished delegates
  mThisDelegates->destroy();
  mThisDelegates.reset();
}

//-----------------------------------------------------------------
void RootPane::OnLoginClick()
{
  mAppUI->GetSession()->GetIdentity()->SetLoginUIDelegate(mThisDelegates);

  std::string uri = mAppUI->GetSession()->GetIdentityURI();
  mAppUI->GetSession()->GetIdentity()->BeginLogin(uri);
  mLoginWebView = mRoot->findChild<WebView*>("webViewLogin");
  mLoginWebView->storage()->clearCache();
  mLoginWebView->setUrl(QUrl("https://app-light.hookflash.me/outer.html"));
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

//  sleep(1);

  mediaEngineInternal->forCallTransport().startVoice();
  mediaEngineInternal->forCallTransport().startVideoChannel();

  QTimer::singleShot(30000, this, SLOT(OnMediaTestButton2Click()));
}

//-----------------------------------------------------------------
void RootPane::OnMediaTestButton2Click()
{
  qDebug() << "***************** RootPane::OnMediaTestButton2Click";

  hookflash::core::internal::MediaEnginePtr mediaEngineInternal = hookflash::core::internal::IMediaEngineForCallTransport::singleton();
  hookflash::core::test::TestMediaEnginePtr testMediaEngineInternal = boost::dynamic_pointer_cast<hookflash::core::test::TestMediaEngine>(mediaEngineInternal);
  hookflash::core::IMediaEnginePtr mediaEngine = hookflash::core::IMediaEngine::singleton();

  mediaEngineInternal->forCallTransport().stopVoice();
  mediaEngineInternal->forCallTransport().stopVideoChannel();

  mediaEngine->stopVideoCapture();

}

//-----------------------------------------------------------------
void RootPane::ProcessFbFriends(const QString& data)
{
  std::string dataString = data.toUtf8().data();
  mAppUI->GetSession()->GetContactsManager()->AddContactsFromJSON(dataString);

  hookflash::core::IdentityURIList identityURIs;
  bool found = mAppUI->GetSession()->GetContactsManager()->prepareIdentityURIListForIdentityLookup(identityURIs);
  if(found) {

    mIdentityLookup = hookflash::core::IIdentityLookup::create(
        mAppUI->GetSession()->GetAccount()->GetCoreAccount(),
        mThisDelegates,
        identityURIs);
  }
}

//-----------------------------------------------------------------
void RootPane::AddContactsToUI()
{
  // Create the data model, specifying sorting keys of "firstName" and "lastName"
  mContactModel = new GroupDataModel(QStringList() << "fullName");

  // Create a JsonDataAccess object and load the .json file. The
  // QDir::currentPath() function returns the current working
  // directory for the app.
//  JsonDataAccess jda;
//  QVariant list = jda.loadFromBuffer(data);

  // Insert the data into the data model. Because the root of the .json file
  // is an array, a QVariant(QVariantList) is returned from load(). You can
  // provide a QVariantList to a data model directly by using insertList().

  const ContactsManager::ContactVector& contacts = mAppUI->GetSession()->GetContactsManager()->GetContactVector();
  int size = contacts.size();

  // TODO: use iterator
  for(int i= 0; i<size; i++) {
    if(contacts.at(i)->GetContact()) {
      QVariantMap itemMap;
      itemMap["id"] = contacts.at(i)->GetIdentityURI().c_str();
      itemMap["fullName"] = contacts.at(i)->GetFullName().c_str();
      itemMap["pictureUrl"] = contacts.at(i)->GetPictureURL().c_str();
      mContactModel->insert(itemMap);
    }
  }

  int n = mContactModel->size();

  // Set the data model for the list view
//  mContactsListView->resetDataModel();
//  model->
  mContactsListView->setDataModel(mContactModel);
}

//-----------------------------------------------------------------
void RootPane::LoginNavigateTo(const std::string& url)
{
  mLoginWebView->setUrl(QUrl(url.c_str()));
}

//-----------------------------------------------------------------
void RootPane::LoginCallJavaScript(const std::string& js)
{
  mLoginJsToEvaluateWhenPageLoaded = js;
  if(mLoginPageHasLoaded) {
    LoginCallJavaScriptAfterPageLoad();
  }
}

//-----------------------------------------------------------------
void RootPane::ContactsNavigateTo(const std::string& url)
{
  if(!mContactsWebView) {
    mContactsWebView = mRoot->findChild<WebView*>("webViewContacts");
  }

  mContactsWebView->setUrl(QUrl(url.c_str()));
}

//-----------------------------------------------------------------
void RootPane::ContactsCallJavaScript(const std::string& js)
{
  mContactsJsToEvaluateWhenPageLoaded = js;
  if(mContactsPageHasLoaded) {
    ContactsCallJavaScriptAfterPageLoad();
  }
}

//-----------------------------------------------------------------
void RootPane::LoginCallJavaScriptAfterPageLoad()
{
  qDebug() << "********* RootPane::LoginCallJavaScriptAfterPageLoad = " << mLoginJsToEvaluateWhenPageLoaded.c_str();

  QString jsq(mLoginJsToEvaluateWhenPageLoaded.c_str());
  mLoginJsToEvaluateWhenPageLoaded.clear();
  bool status = mLoginWebView->evaluateJavaScript(jsq);
}

//-----------------------------------------------------------------
void RootPane::ContactsCallJavaScriptAfterPageLoad()
{
  qDebug() << "********* RootPane::ContactsCallJavaScriptAfterPageLoad = " << mContactsJsToEvaluateWhenPageLoaded.c_str();

  if(!mContactsWebView) {
    mContactsWebView = mRoot->findChild<WebView*>("webViewContacts");
  }

  QString jsq(mContactsJsToEvaluateWhenPageLoaded.c_str());
  mContactsJsToEvaluateWhenPageLoaded.clear();
  bool status = mContactsWebView->evaluateJavaScript(jsq);
}

//-----------------------------------------------------------------
void RootPane::onIdentityLookupCompleted(hookflash::core::IIdentityLookupPtr lookup)
{
  mAppUI->GetSession()->GetContactsManager()->handleIdentityLookupResult(lookup->getIdentities());
  mIdentityLookup.reset();

  hookflash::core::ContactList contactList;
  mAppUI->GetSession()->GetContactsManager()->prepareContactListForContactPeerFilePublicLookup(contactList);

  mContactPeerFilePublicLookup = hookflash::core::IContactPeerFilePublicLookup::create(mThisDelegates, contactList);
}

//-----------------------------------------------------------------
// IContactPeerFilePublicLookupDelegate
void RootPane::onContactPeerFilePublicLookupCompleted(hookflash::core::IContactPeerFilePublicLookupPtr lookup)
{
  AddContactsToUI();
}

//-----------------------------------------------------------------
void RootPane::onLayoutFrameChanged(const QRectF &layoutFrame) {
  if(!mVideoRenderer) {
    mVideoWindowSize = layoutFrame;
    CreateVideoRenderer();
//    OnMediaTestButton1Click();
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

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void RootPaneDelegates::onIdentityLookupCompleted(hookflash::core::IIdentityLookupPtr lookup)
{
  if (!mOuter) return;
  mOuter->onIdentityLookupCompleted(lookup);
}

void RootPaneDelegates::onContactPeerFilePublicLookupCompleted(hookflash::core::IContactPeerFilePublicLookupPtr lookup)
{
  if (!mOuter) return;
  mOuter->onContactPeerFilePublicLookupCompleted(lookup);
}

