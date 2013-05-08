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
#include <bb/cascades/WebLoadStatus>
#include <bb/system/SystemToast>

#include <QDebug>
#include <QTimer>
#include <iostream>

#include <hookflash/core/internal/core_MediaEngine.h>
#include <hookflash/core/test/TestMediaEngine.h>

using namespace bb::cascades;
using namespace bb::data;
using namespace bb::system;
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
    mContactsPageHasLoaded(false),
    mRoot(NULL),
    mForeignWindow(NULL),
    mLoginWebView(NULL),
    mContactsWebView(NULL),
    mContactsListView(NULL),
    mArrayContactsModel(NULL),
    mGroupContactsModel(NULL)
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

  // If the peer files exist then go straigth to logged in state.
  if(peerFile.size() > 0 && secret.size() > 0) {
    bool success = mAppUI->GetSession()->GetAccount()->Relogin(peerFile, secret);
    if(success) {
      QObject* tabbedPaneMain = mRoot->findChild<QObject*>("tabContacts");
      QMetaObject::invokeMethod(tabbedPaneMain, "loginSuccessful",  Qt::DirectConnection);
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
void RootPane::BeginLogin()
{
  mLoginWebView = mRoot->findChild<WebView*>("webViewLogin");
  mLoginWebView->storage()->clearCache();

  mAppUI->GetSession()->GetIdentity()->SetLoginUIDelegate(mThisDelegates);

  std::string uri = mAppUI->GetSession()->GetIdentityURI();
  mAppUI->GetSession()->GetIdentity()->BeginLogin(uri);
  mLoginWebView->setUrl(QUrl("https://app-light.hookflash.me/outer.html"));
}

//-----------------------------------------------------------------
bool RootPane::OnLoginNavigationRequested(QUrl url)
{
  QString urlFull = url.toString();
  qDebug() << "******** OnNavigationRequested = " << urlFull;

  QString methodValue = url.queryItemValue("method");
  if(methodValue.length() != 0) {
    if(methodValue.indexOf("notifyClient", 0) == 0) {
      std::string fullDataAsStdString = methodValue.toUtf8().constData();
      std::string dataAsStdString = fullDataAsStdString.substr(strlen("notifyClient;data="));

      mAppUI->GetSession()->GetIdentity()->OnNotifyClient(dataAsStdString);
      return true; // Cancel
    }
    else if(methodValue.indexOf("proccessMyFBProfile", 0) == 0) {
      std::string fullDataAsStdString = methodValue.toUtf8().constData();
      std::string dataAsStdString = fullDataAsStdString.substr(strlen("proccessMyFBProfile;data="));

      mAppUI->GetSession()->GetAccount()->ProcessMyFBProfile(dataAsStdString);
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
    return mAppUI->GetSession()->GetIdentity()->OnWebBrowserPageNavigation(urlAsStdString);
  }
}

//-----------------------------------------------------------------
void RootPane::OnLoginLoadingChanged(int status, QString url)
{
  if(WebLoadStatus::Succeeded == status) {
    mLoginPageHasLoaded = true;
    if(mLoginJsToEvaluateWhenPageLoaded.size() > 0) {
      LoginCallJavaScriptAfterPageLoad();
    }
  }
  qDebug() << "*** OnLoadChanged = " << url;
}


//-----------------------------------------------------------------
bool RootPane::OnContactsNavigationRequested(QUrl url)
{
  QString urlFull = url.toString();
  qDebug() << "******** OnNavigationRequested = " << urlFull;

  QString methodValue = url.queryItemValue("method");
  if(methodValue.length() != 0) {
    if(methodValue.indexOf("proccessFbFriends", 0) == 0) {
      QString fullData = methodValue.mid(strlen("proccessFbFriends;data="));

      WriteContacts(fullData);
      ProcessFbFriends(fullData);
      return true; // Cancel
    }
    else {
      qDebug() << "*** RootPane::OnNavigationRequested did not process method type = " << methodValue;
      return true;
    }
  }
  else {
    QString urlFull = url.toString();
    std::string urlAsStdString = urlFull.toUtf8().constData();
    return mAppUI->GetSession()->GetIdentity()->OnWebBrowserPageNavigation(urlAsStdString);
  }
}

//-----------------------------------------------------------------
void RootPane::OnContactsLoadingChanged(int status, QString url)
{
  if(WebLoadStatus::Succeeded == status) {
    mContactsPageHasLoaded = true;
    if(mContactsJsToEvaluateWhenPageLoaded.size() > 0) {
      ContactsCallJavaScriptAfterPageLoad();
    }
  }

  std::string statusString("FAILED");
  if(0 == status) { statusString = "Started"; }
  else if(1 == status) { statusString = "Completed with success"; }
  qDebug() << "*** OnLoadChanged = " << url << " status=" << statusString.c_str();
}


//-----------------------------------------------------------------
bool RootPane::StartCall(QString currentRemoteUserId, bool hasVideo)
{
  AccountPtr account = mAppUI->GetSession()->GetAccount();
  ContactPtr contact = account->GetContactByFacebookID((const char*) currentRemoteUserId.toUtf8().data());
  if(contact) {
	account->PlaceCallTo(contact, true, hasVideo);
	return true;
  }
  return false;
}

//-----------------------------------------------------------------
void RootPane::EndCall()
{
  AccountPtr account = mAppUI->GetSession()->GetAccount();
  if(account)
	  account->GetActiveCall()->hangup(hookflash::core::ICall::CallClosedReason_None);
}

//-----------------------------------------------------------------
void RootPane::OnVideoCallWindowOpened()
{
  mCallWindowIsOpen = true;
  if(!mVideoRenderer && !mVideoWindowSize.isNull()) {
    CreateVideoRenderer();
  }
  else {
    // TODO:    // UnpauseVideoRenderer();
  }
}

//-----------------------------------------------------------------
void RootPane::OnVideoCallWindowClosed()
{
  mCallWindowIsOpen = false;
  if(mVideoRenderer) {
// TODO:  // PauseVideoRenderer();
  }
}

//-----------------------------------------------------------------
bool RootPane::SendTextMessage(QString currentTextUserId, QString text) {

  AccountPtr account = mAppUI->GetSession()->GetAccount();
  ContactPtr contact = account->GetContactByFacebookID((const char*) currentTextUserId.toUtf8().data());
  if(contact) {
	updateChatWindow("Me: " + text);
    account->SendMessageTo(contact, (const char*) text.toUtf8().data());
    return true;
  }
  return false;
}

//-----------------------------------------------------------------
void RootPane::IncomingTextMessage(QString incomingText)
{
	updateChatWindow(incomingText);
}

//-----------------------------------------------------------------
void RootPane::updateChatWindow(const QString msg)
    {
        if (mChatHistory.size() > 0)
        	mChatHistory.append("\n");

        mChatHistory.append(msg);

        emit chatHistoryChanged();
    }

#if 0
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
#endif

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
  if(!mArrayContactsModel) {
    // Create the data model, specifying sorting keys of "firstName" and "lastName"
    mGroupContactsModel = new GroupDataModel(QStringList() << "fullName" << "pictureUrl");
    mArrayContactsModel = new ArrayDataModel();

    const ContactsManager::ContactVector& contacts = mAppUI->GetSession()->GetContactsManager()->GetContactVector();
    int size = contacts.size();

    qDebug() << "*** RootPane::AddContactsToUI: " << size << "contacts to import";

    // TODO: use iterator
    for(int i= 0; i<size; i++) {
      if(contacts.at(i)->GetContact()) {
        QVariantMap itemMap;
        itemMap["id"] = contacts.at(i)->GetIdentityURI().c_str();
        itemMap["fullName"] = contacts.at(i)->GetFullName().c_str();
        itemMap["pictureUrl"] = contacts.at(i)->GetPictureURL().c_str();
        mGroupContactsModel->insert(itemMap);
        mArrayContactsModel->append(itemMap);
      }
    }

  //  mContactsListView->setDataModel(mContactModel);
    mContactsListView->setDataModel(mGroupContactsModel);

    QObject* tabContacts = mRoot->findChild<QObject*>("tabContacts");
    QMetaObject::invokeMethod(tabContacts, "activityLoadingDone",  Qt::DirectConnection);

  }
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
void RootPane::LoginMakeBrowserWindowVisible()
{
  QObject* container = mRoot->findChild<QObject*>("containerWebView");
  qDebug() << "*** RootPane::LoginMakeBrowserWindowVisible";
  QMetaObject::invokeMethod(container, "showBrowser",  Qt::DirectConnection);
}

//-----------------------------------------------------------------
void RootPane::LoginHideBrowserAfterLogin()
{
  QObject* container = mRoot->findChild<QObject*>("containerWebView");
  QVariant msg = "Processing Login...";
  QMetaObject::invokeMethod(container, "setLabelText",  Qt::DirectConnection, Q_ARG(QVariant, msg));
  QMetaObject::invokeMethod(container, "hideBrowser",  Qt::DirectConnection);
}

//-----------------------------------------------------------------
void RootPane::ContactsNavigateTo(const std::string& url)
{
  qDebug() << "*** RootPane::ContactsNavigateTo to " << url.c_str();
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
	qWarning() << "********* RootPane::onIdentityLookupCompleted   ";
  //ZS_LOG_DEBUG(log("********* RootPane::onIdentityLookupCompleted") );
  mAppUI->GetSession()->GetContactsManager()->handleIdentityLookupResult(lookup->getIdentities());
  mIdentityLookup.reset();

  hookflash::core::ContactList contactList;
  mAppUI->GetSession()->GetContactsManager()->prepareContactListForContactPeerFilePublicLookup(contactList);

  qWarning() << "********* RootPane::IContactPeerFilePublicLookup called   ";
  //ZS_LOG_DEBUG(log("********* RootPane::IContactPeerFilePublicLookup called") );
  mContactPeerFilePublicLookup = hookflash::core::IContactPeerFilePublicLookup::create(mThisDelegates, contactList);
}

//-----------------------------------------------------------------
// IContactPeerFilePublicLookupDelegate
void RootPane::onContactPeerFilePublicLookupCompleted(hookflash::core::IContactPeerFilePublicLookupPtr lookup)
{
	qWarning() << "********* RootPane::onContactPeerFilePublicLookupCompleted   ";
  //ZS_LOG_DEBUG(log("********* RootPane::onContactPeerFilePublicLookupCompleted") );
  AddContactsToUI();
  qWarning() << "********* RootPane::AddContactsToUI completed   ";
  //ZS_LOG_DEBUG(log("********* RootPane::AddContactsToUI completed") );
  int i = 0;
  i++;
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
void RootPane::WriteContacts(QString rawContacts)
{
  QString homePath = QDir::homePath();
  QString contactsPath = homePath+"/contacts.txt";

  QFile contactsFile(contactsPath);
  contactsFile.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out(&contactsFile);
  out << rawContacts;
  contactsFile.close();
}

//-----------------------------------------------------------------
std::string RootPane::ReadContacts()
{
  std::string fileContents;
  QString homePath = QDir::homePath();
  QString contactsPath = homePath+"/contacts.txt";
  QFile contactsFile(contactsPath);
  if (contactsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&contactsFile);
    QString contents = stream.readAll();
    fileContents = contents.toUtf8().constData();
  }
  contactsFile.close();
  return fileContents;
}


//-----------------------------------------------------------------
void RootPane::LoginSuccessful() {
  QObject* tabContacts = mRoot->findChild<QObject*>("tabContacts");
  QMetaObject::invokeMethod(tabContacts, "loginSuccessful",  Qt::DirectConnection);

  QString storedContacts = ReadContacts().c_str();
  if (storedContacts.size() > 0)
  {
	  ProcessFbFriends(storedContacts);
  }
  else
  {
	  ContactsNavigateTo(mAppUI->GetSession()->GetContactsURL());
  }
}

//-----------------------------------------------------------------
void RootPane::LoginFailed() {
  QObject* tabContacts = mRoot->findChild<QObject*>("tabContacts");
  QMetaObject::invokeMethod(tabContacts, "loginFailed",  Qt::DirectConnection);
}

//-----------------------------------------------------------------
void RootPane::ShowNewMessage(ContactPtr contact, const char* message) {
  qDebug() << "*********************** RootPane::ShowNewMessage";
//  SystemToast* toast = new SystemToast();
//  SystemUiButton* toastButton = toast->button();
//
//  toast->setBody(message);
//  toastButton->setLabel("View");
//  toast->show();

  QString mess = message;
  QString name = contact->GetFullName().c_str();

  QString displayMessage = name + ": "+ mess;

  IncomingTextMessage(displayMessage);
}

QString RootPane::chatHistory() const
{
    return mChatHistory;
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

