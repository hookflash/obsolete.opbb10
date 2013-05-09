#include "account.h"
#include "session.h"
#include "userIdentity.h"
#include "applicationui.h"
#include "rootPane.h"
#include "contactsManager.h"
#include "contact.h"

#include <QFile>
#include <QDir>
#include <QDebug>

#include <hookflash/core/IHelper.h>
#include <hookflash/core/ICall.h>
#include <hookflash/stack/IServiceIdentity.h>
#include <hookflash/stack/IHelper.h>

#include <zsLib/Stringize.h>
#include <zsLib/Log.h>
#include <zsLib/helpers.h>

namespace hookflash { namespace blackberry { ZS_DECLARE_SUBSYSTEM(hookflash_blackberry) } }

using namespace hookflash::blackberry;
using zsLib::String;
using zsLib::Stringize;
using zsLib::Time;
using zsLib::XML::ElementPtr;
using hookflash::stack::IServiceIdentity;
using hookflash::core::ConversationThreadList;
using hookflash::core::ConversationThreadListPtr;
using hookflash::core::IConversationThread;
using hookflash::core::IConversationThreadPtr;
using hookflash::core::ContactList;
using hookflash::core::ContactListPtr;
using hookflash::core::ContactProfileInfoList;
using hookflash::core::ContactProfileInfo;
using hookflash::core::ICall;
using hookflash::core::ICallPtr;
using hookflash::core::IContact;
using hookflash::core::IContactPtr;

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
AccountPtr Account::CreateInstance(SessionPtr session)
{
  AccountPtr pThis(new Account(session));
  pThis->mThisWeak = pThis;
  return pThis;
}

//-------------------------------------------------------------------------
Account::Account(SessionPtr session) :
  mID(zsLib::createPUID()),
  mSession(session)
{
  ZS_LOG_DEBUG(log("created"))
}

Account::~Account()
{
  mThisWeak.reset();
  ZS_LOG_DEBUG(log("destroyed"))
}


//-------------------------------------------------------------------------
void Account::Login()
{
  mOpAccount = hookflash::core::IAccount::login(
                                                mThisWeak.lock(),
                                                mThisWeak.lock(),
                                                mThisWeak.lock(),
                                                mSession->GetPeerContactServiceDomain().c_str(),
                                                mSession->GetIdentity()->GetCoreIdentity()
                                                );

  if(mOpAccount) {
    ZS_LOG_DETAIL(log("hookflash::core::IAccount object created"))
  } else {
    ZS_LOG_ERROR(Basic, log("failed to create hookflash::core::IAccount object"))
  }
}

//-------------------------------------------------------------------------
bool Account::Relogin(const std::string& peerFile, const std::string& secret)
{
  zsLib::XML::ElementPtr privatePeerFileElement = hookflash::core::IHelper::createFromString(peerFile);

  mOpAccount = hookflash::core::IAccount::relogin(
      mThisWeak.lock(),
      mThisWeak.lock(),
      mThisWeak.lock(),
      privatePeerFileElement,
      secret.c_str());
  if(mOpAccount) {
    qDebug() << "Account::Login: Account object created";
    return true;
  }
  qDebug() << "ERROR: Account::Login: Account object creation failed";
  return false;
}

void Account::ProcessMyFBProfile(const std::string& data)
{

}

void Account::ProcessFbFriends(const std::string& data)
{

}

std::string Account::ReadPrivatePeerFile()
{
  std::string fileContents;
  QString homePath = QDir::homePath();
  QString privatePeerPath = homePath+"/privatePeer.txt";
  QFile privatePeer(privatePeerPath);
  if (privatePeer.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&privatePeer);
    QString contents = stream.readAll();
    fileContents = contents.toUtf8().constData();
  }
  privatePeer.close();
  return fileContents;
}

std::string Account::ReadPrivatePeerSecretFile()
{
  std::string fileContents;
  QString homePath = QDir::homePath();
  QString privatePeerSecretPath = homePath+"/privatePeerSecret.txt";
  QFile privatePeer(privatePeerSecretPath);
  if (privatePeer.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&privatePeer);
    QString contents = stream.readAll();
    fileContents = contents.toUtf8().constData();
  }
  privatePeer.close();
  return fileContents;
}

void Account::WritePeerFiles()
{
  QString homePath = QDir::homePath();
  QString privatePeerPath = homePath+"/privatePeer.txt";
  QString privatePeerSecretPath = homePath+"/privatePeerSecret.txt";
  zsLib::String privatePeerFile = hookflash::core::IHelper::convertToString(mOpAccount->savePeerFilePrivate());

  QFile privatePeer(privatePeerPath);
  privatePeer.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out(&privatePeer);
  out << privatePeerFile.c_str();
  privatePeer.close();

  hookflash::stack::SecureByteBlockPtr secure = mOpAccount->getPeerFilePrivateSecret();
  byte* secureInBytes = secure->BytePtr();

  QFile privatePeerSecret(privatePeerSecretPath);
  privatePeerSecret.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream outSecret(&privatePeerSecret);
  outSecret << (const char*) secureInBytes;
  privatePeerSecret.close();
}

ContactPtr Account::GetContactByFacebookID(const char *facebookID)
{
  ZS_LOG_DEBUG(log("searching for facebook contact") + ", identity uri=" + facebookID)

  ContactsManagerPtr contactManager = mSession->GetContactsManager();
  return contactManager->FindContactByIdentityURI(facebookID);
}

ContactPtr Account::GetActiveCallContact()
{
  if (!mCall) {
    ZS_LOG_WARNING(Detail, log("call is not active"))
    return ContactPtr();
  }

  hookflash::core::IContactPtr remoteContact;
  remoteContact = mCall->getCaller();
  if (remoteContact->isSelf())
    remoteContact = mCall->getCallee();

  ContactsManagerPtr contactManager = mSession->GetContactsManager();
  return contactManager->FindContactBy(remoteContact);
}

bool Account::PlaceCallTo(
    ContactPtr contact,
    bool includeAudio,
    bool includeVideo
    )
{
  if (!contact) {
    ZS_LOG_ERROR(Debug, log("cannot place call to this contact as contact does not exist"))
    return false;
  }

  if (mCall) {
    mCall->hangup(ICall::CallClosedReason_User);
    ZS_LOG_WARNING(Debug, log("sample only supports one active call at a time"))
    return false;
  }

  hookflash::core::IContactPtr coreContact = contact->GetContact();
  if (!coreContact) {
    ZS_LOG_WARNING(Debug, log("the contact specified is not a core contact") + ", identity=" + contact->GetIdentityURI())
    return false;
  }
  if (!coreContact->hasPeerFilePublic()) {
    ZS_LOG_WARNING(Debug, log("cannot place call to contact that does not have a public peer file") + ", identity=" + contact->GetIdentityURI())
    return false;
  }

  IConversationThreadPtr useConversation = getOrCreateConversationThreadFor(coreContact);
  ZS_THROW_BAD_STATE_IF(!useConversation)

  if(includeVideo)
  {
	  hookflash::core::IMediaEnginePtr mediaEngine = hookflash::core::IMediaEngine::singleton();
	  mediaEngine->setChannelRenderView(GetSession()->GetAppUI()->GetRootPane()->GetRenderWindow().get());
  }

  mCall = ICall::placeCall(useConversation, coreContact, includeAudio, includeVideo);
}

void Account::HandleCallState(hookflash::core::ICall::CallStates state)
{
  // TODO: wire up
	if(state == hookflash::core::ICall::CallState_Preparing)
	{
		return;
	}
	else
	{
		if (!mCall)
		{
			GetSession()->GetAppUI()->GetRootPane()->OnCallEnded();
			return;
		}
		ContactsManagerPtr contactManager = mSession->GetContactsManager();
		ContactPtr callerContact = contactManager->FindContactBy(mCall->getCaller());
		ContactPtr calleeContact = contactManager->FindContactBy(mCall->getCallee());

	    if (!callerContact && !calleeContact) {
		  ZS_LOG_WARNING(Detail, log("message received but contact was not found in contacts manager"))
		  return;
	    }

		GetSession()->GetAppUI()->GetRootPane()->HandleCall(callerContact, calleeContact, ICall::toString(state));
	}
  // WARNING: if call is hanging up/hung up no active user/call will be present
}

bool Account::SendMessageTo(
    ContactPtr contact,
    const char *text
    )
{
  ZS_THROW_INVALID_ARGUMENT_IF(!text)

  if (!contact) {
    ZS_LOG_ERROR(Debug, log("cannot send message to this contact as contact does not exist"))
    return false;
  }

  hookflash::core::IContactPtr coreContact = contact->GetContact();
  if (!coreContact) {
    ZS_LOG_WARNING(Debug, log("the contact specified is not a core contact") + ", identity=" + contact->GetIdentityURI())
    return false;
  }
  if (!coreContact->hasPeerFilePublic()) {
    ZS_LOG_WARNING(Debug, log("cannot send message to contact that does not have a public peer file") + ", identity=" + contact->GetIdentityURI())
    return false;
  }

  ZS_LOG_DEBUG(log("attempting to send message") + ", message=" + text + IContact::toDebugString(coreContact))

  string str(text);
  if ("ring" == str) {
    ICallPtr call = GetActiveCall();
    if (call) {
      call->ring();
      return true;
    }
  }
  if ("answer" == str) {
    ICallPtr call = GetActiveCall();
    if (call) {
      call->answer();
      return true;
    }
  }
  if ("hangup" == str) {
    ICallPtr call = GetActiveCall();
    if (call) {
      call->hangup();
      return true;
    }
  }
  if ("audio" == str) {
    PlaceCallTo(contact, true, false);
    return true;
  }
  if ("video" == str) {
    PlaceCallTo(contact, true, true);
    return true;
  }

  IConversationThreadPtr useConversation = getOrCreateConversationThreadFor(coreContact);
  ZS_THROW_BAD_STATE_IF(!useConversation)

  useConversation->sendMessage(hookflash::stack::IHelper::randomString(32), "text/x-application-hookflash-message-text", text);
  return true;
}

void Account::HandleMessageFrom(
    ContactPtr contact,
    const char *message
    )
{
  qDebug() << "************************** Account::HandleMessageFrom";
  ZS_THROW_INVALID_ARGUMENT_IF(!contact)
  ZS_THROW_INVALID_ARGUMENT_IF(!message)

  GetSession()->GetAppUI()->GetRootPane()->ShowNewMessage(contact, message);
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void Account::onAccountStateChanged(hookflash::core::IAccountPtr account, AccountStates state)
{
  if(hookflash::core::IAccount::AccountState_Ready) {
    qDebug() << "AccountDelegate::onAccountStateChanged: Ready";
    WritePeerFiles();
    GetSession()->GetAppUI()->GetRootPane()->LoginSuccessful();
  }
}

//-------------------------------------------------------------------------
void Account::onAccountAssociatedIdentitiesChanged(hookflash::core::IAccountPtr account)
{
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void Account::onConversationThreadNew(hookflash::core::IConversationThreadPtr conversationThread)
{

}

//-------------------------------------------------------------------------
void Account::onConversationThreadContactsChanged(hookflash::core::IConversationThreadPtr conversationThread)
{

}

//-------------------------------------------------------------------------
void Account::onConversationThreadContactStateChanged(
                                                     hookflash::core::IConversationThreadPtr conversationThread,
                                                     hookflash::core::IContactPtr contact,
                                                     hookflash::core::IConversationThread::ContactStates state)
{

}

//-------------------------------------------------------------------------
void Account::onConversationThreadMessage(
                                         hookflash::core::IConversationThreadPtr conversationThread,
                                         const char *messageID)
{
  ContactListPtr contacts = conversationThread->getContacts();
  ZS_THROW_BAD_STATE_IF(!contacts)

  ZS_LOG_DEBUG(log("message received") + ", message ID=" + messageID + IConversationThread::toDebugString(conversationThread))

  IContactPtr contact;
  String messageTypeIgnored;
  String message;
  Time time;

  bool found = conversationThread->getMessage(
      messageID,
      contact,
      messageTypeIgnored,
      message,
      time
      );

  if (!found) {
    ZS_LOG_WARNING(Detail, log("message received with ID that was not found"))
    return;
  }

  ZS_LOG_DEBUG(log("message") + ", message=" + message)

  if (!contact) {
    ZS_LOG_WARNING(Detail, log("message received but other contact was not found"))
    return;
  }

  if (contact->isSelf()) {
    ZS_LOG_WARNING(Detail, log("sample application does not handle messages sent by self on another device right now"))
    return;
  }

  ContactsManagerPtr contactManager = mSession->GetContactsManager();
  ContactPtr actualContact = contactManager->FindContactBy(contact);

  if (!actualContact) {
    ZS_LOG_WARNING(Detail, log("message received but contact was not found in contacts manager"))
    return;
  }

  HandleMessageFrom(actualContact, message);
}

//-------------------------------------------------------------------------
void Account::onConversationThreadMessageDeliveryStateChanged(
                                                             hookflash::core::IConversationThreadPtr conversationThread,
                                                             const char *messageID,
                                                             hookflash::core::IConversationThread::MessageDeliveryStates state)
{

}

//-------------------------------------------------------------------------
void Account::onConversationThreadPushMessage(
                                             hookflash::core::IConversationThreadPtr conversationThread,
                                             const char *messageID,
                                             hookflash::core::IContactPtr contact)
{

}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void Account::onCallStateChanged(hookflash::core::ICallPtr call, hookflash::core::ICall::CallStates state)
{
  if (!mCall) {
    mCall = call;
  }

  if (mCall != call) {
    call->hangup(ICall::CallClosedReason_Decline);
    ZS_LOG_WARNING(Detail, log("declining call since it is not the active call"))
    return;
  }

  switch (state) {
    case ICall::CallState_None:           break;
    case ICall::CallState_Preparing:      break;
    case ICall::CallState_Incoming:
    	mCall->ring();
    	break;
    case ICall::CallState_Placed:         break;
    case ICall::CallState_Early:          break;
    case ICall::CallState_Ringing:
    	//mCall->answer();
    	break;
    case ICall::CallState_Ringback:       break;
    case ICall::CallState_Open:           break;
    case ICall::CallState_Active:         break;
    case ICall::CallState_Inactive:       break;
    case ICall::CallState_Hold:           break;
    case ICall::CallState_Closing:
    case ICall::CallState_Closed:         {
      mCall.reset();
      break;
    }
  }

  HandleCallState(state);
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
String Account::log(const char *message) const
{
  return String("blackberry::Account [") + Stringize<typeof(mID)>(mID).string() + "] " + message;
}

//-------------------------------------------------------------------------
IConversationThreadPtr Account::getOrCreateConversationThreadFor(IContactPtr contact)
{
  IConversationThreadPtr useConversation;

  ConversationThreadListPtr conversations = IConversationThread::getConversationThreads(mOpAccount);

  if (conversations) {
    for (ConversationThreadList::iterator iter = conversations->begin(); iter != conversations->end(); ++iter) {
      IConversationThreadPtr conversation = (*iter);
      ZS_THROW_BAD_STATE_IF(!conversation)

      ContactListPtr contacts = conversation->getContacts();
      ZS_THROW_BAD_STATE_IF(!contacts)

      for (ContactList::iterator iterContact = contacts->begin(); iterContact != contacts->end(); ++iterContact)
      {
        hookflash::core::IContactPtr otherContact = (*iterContact);
        if (otherContact == contact) {
          useConversation = conversation;
          ZS_LOG_DEBUG(log("found conversation thread to use for contact") + IContact::toDebugString(contact))
          break;
        }
      }
      if (useConversation) {
        ZS_LOG_DEBUG(log("found conversation thread to use"))
        break;
      }
    }
  }

  if (!useConversation) {

    useConversation = IConversationThread::create(mOpAccount, ElementPtr());

    ContactProfileInfoList contacts;
    ContactProfileInfo remoteInfo;
    remoteInfo.mContact = contact;
    contacts.push_back(remoteInfo);

    useConversation->addContacts(contacts);
  }

  return useConversation;
}

