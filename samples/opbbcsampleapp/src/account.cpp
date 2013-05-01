#include "account.h"
#include "session.h"
#include "userIdentity.h"
#include <hookflash/core/IHelper.h>
#include <QFile>
#include <QDir>
#include <QDebug>

using namespace hookflash::blackberry;

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
boost::shared_ptr<Account> Account::CreateInstance(boost::shared_ptr<Session> session)
{
  boost::shared_ptr<Account> instance(new Account(session));
  instance->mWeakThis = instance;
  return instance;
}

//-------------------------------------------------------------------------
Account::Account(boost::shared_ptr<Session> session) : mSession(session)
{
}

//-------------------------------------------------------------------------
void Account::Login()
{
  mDelegate = boost::shared_ptr<AccountDelegate>(new AccountDelegate(mWeakThis.lock()));
  mConversationThreadDelegate = boost::shared_ptr<ConversationThreadDelegate>(new ConversationThreadDelegate(mWeakThis.lock()));
  mCallDelegate = boost::shared_ptr<CallDelegate>(new CallDelegate(mWeakThis.lock()));

  mOpAccount = hookflash::core::IAccount::login(
      mDelegate,
      mConversationThreadDelegate,
      mCallDelegate,
      mSession->GetPeerContactServiceDomain().c_str(),
      mSession->GetIdentity()->GetCoreIdentity());
  if(mOpAccount) {
    qDebug() << "Account::Login: Account object created";
  }
  else {
    qDebug() << "ERROR: Account::Login: Account object creation failed";
  }
}

//-------------------------------------------------------------------------
bool Account::Relogin(const std::string& peerFile, const std::string& secret)
{
  mDelegate = boost::shared_ptr<AccountDelegate>(new AccountDelegate(mWeakThis.lock()));
  mConversationThreadDelegate = boost::shared_ptr<ConversationThreadDelegate>(new ConversationThreadDelegate(mWeakThis.lock()));
  mCallDelegate = boost::shared_ptr<CallDelegate>(new CallDelegate(mWeakThis.lock()));

  zsLib::XML::ElementPtr privatePeerFileElement = hookflash::core::IHelper::createFromString(peerFile);

  mOpAccount = hookflash::core::IAccount::relogin(
      mDelegate,
      mConversationThreadDelegate,
      mCallDelegate,
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

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
AccountDelegate::AccountDelegate(boost::shared_ptr<Account> parentAccount) : mParentAccount(parentAccount)
{

}

//-------------------------------------------------------------------------
AccountDelegate::~AccountDelegate()
{
}

//-------------------------------------------------------------------------
void AccountDelegate::onAccountStateChanged(hookflash::core::IAccountPtr account, AccountStates state)
{
  if(hookflash::core::IAccount::AccountState_Ready) {
    qDebug() << "AccountDelegate::onAccountStateChanged: Ready";
    mParentAccount.lock()->WritePeerFiles();
    mParentAccount.lock()->GetSession()->GetIdentity()->OnAccountStateReady();
  }
}

//-------------------------------------------------------------------------
void AccountDelegate::onAccountAssociatedIdentitiesChanged(hookflash::core::IAccountPtr account)
{

}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

ConversationThreadDelegate::ConversationThreadDelegate(boost::shared_ptr<Account> parentAccount) : mParentAccount(parentAccount)
{

}

//-------------------------------------------------------------------------
ConversationThreadDelegate::~ConversationThreadDelegate()
{
}

//-------------------------------------------------------------------------
void ConversationThreadDelegate::onConversationThreadNew(hookflash::core::IConversationThreadPtr conversationThread)
{

}

//-------------------------------------------------------------------------
void ConversationThreadDelegate::onConversationThreadContactsChanged(hookflash::core::IConversationThreadPtr conversationThread)
{

}

//-------------------------------------------------------------------------
void ConversationThreadDelegate::onConversationThreadContactStateChanged(
                                                     hookflash::core::IConversationThreadPtr conversationThread,
                                                     hookflash::core::IContactPtr contact,
                                                     hookflash::core::IConversationThread::ContactStates state)
{

}

//-------------------------------------------------------------------------
void ConversationThreadDelegate::onConversationThreadMessage(
                                         hookflash::core::IConversationThreadPtr conversationThread,
                                         const char *messageID)
{

}

//-------------------------------------------------------------------------
void ConversationThreadDelegate::onConversationThreadMessageDeliveryStateChanged(
                                                             hookflash::core::IConversationThreadPtr conversationThread,
                                                             const char *messageID,
                                                             hookflash::core::IConversationThread::MessageDeliveryStates state)
{

}

//-------------------------------------------------------------------------
void ConversationThreadDelegate::onConversationThreadPushMessage(
                                             hookflash::core::IConversationThreadPtr conversationThread,
                                             const char *messageID,
                                             hookflash::core::IContactPtr contact)
{

}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

CallDelegate::CallDelegate(boost::shared_ptr<Account> parentAccount) : mParentAccount(parentAccount)
{

}

//-------------------------------------------------------------------------
CallDelegate::~CallDelegate()
{
}

//-------------------------------------------------------------------------
void CallDelegate::onCallStateChanged(hookflash::core::ICallPtr call, hookflash::core::ICall::CallStates state)
{

}
