#include "account.h"
#include "session.h"
#include "userIdentity.h"
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

void Account::ProcessMyFBProfile(const std::string& data)
{

}

void Account::ProcessFbFriends(const std::string& data)
{

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
