#include "account.h"
#include "session.h"
#include "userIdentity.h"
#include "applicationui.h"
#include "rootPane.h"
#include "loginPane.h"

#include <hookflash/core/IHelper.h>
#include <QFile>
#include <QDir>
#include <QDebug>

using namespace hookflash::blackberry;

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
Account::Account(SessionPtr session) : mSession(session)
{
}

//-------------------------------------------------------------------------
void Account::Login()
{
  mOpAccount = hookflash::core::IAccount::login(
      mThisWeak.lock(),
      mThisWeak.lock(),
      mThisWeak.lock(),
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

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void Account::onAccountStateChanged(hookflash::core::IAccountPtr account, AccountStates state)
{
  if(hookflash::core::IAccount::AccountState_Ready) {
    qDebug() << "AccountDelegate::onAccountStateChanged: Ready";
    WritePeerFiles();
    GetSession()->GetAppUI()->GetRootPane()->GetLoginPane()->NavigateTo(GetSession()->GetContactsURL());
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

}
