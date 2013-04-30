#ifndef HFBB_ACCOUNT_H
#define HFBB_ACCOUNT_H

#include "hookflash/core/IAccount.h"
#include "hookflash/core/IConversationThread.h"
#include "hookflash/core/ICall.h"

namespace hookflash {
  namespace blackberry {

    class Session;
    class AccountDelegate;
    class ConversationThreadDelegate;
    class CallDelegate;

    //-------------------------------------------------------------------------------
    class Account
    {
    public:
      static boost::shared_ptr<Account> CreateInstance(boost::shared_ptr<Session> session);
      virtual ~Account() {}

      void Login();
      void ProcessMyFBProfile(const std::string& data);
      void ProcessFbFriends(const std::string& data);
      boost::shared_ptr<Session> GetSession() { return mSession; }

    private:
      Account(boost::shared_ptr<Session> session);

      boost::weak_ptr<Account> mWeakThis;
      boost::shared_ptr<Session> mSession;
      boost::shared_ptr<hookflash::core::IAccount> mOpAccount;
      boost::shared_ptr<AccountDelegate> mDelegate;
      boost::shared_ptr<ConversationThreadDelegate> mConversationThreadDelegate;
      boost::shared_ptr<CallDelegate> mCallDelegate;
    };

    //-------------------------------------------------------------------------------
    class AccountDelegate : public hookflash::core::IAccountDelegate
    {
    public:
      AccountDelegate(boost::shared_ptr<Account> parentAccount);
      virtual ~AccountDelegate();

      virtual void onAccountStateChanged(hookflash::core::IAccountPtr account, AccountStates state);
      virtual void onAccountAssociatedIdentitiesChanged(hookflash::core::IAccountPtr account);

    private:
      boost::weak_ptr<Account> mParentAccount;
    };

    //-------------------------------------------------------------------------------
    class ConversationThreadDelegate : public hookflash::core::IConversationThreadDelegate
    {
    public:
      ConversationThreadDelegate(boost::shared_ptr<Account> parentAccount);
      virtual ~ConversationThreadDelegate();

      virtual void onConversationThreadNew(hookflash::core::IConversationThreadPtr conversationThread);

      virtual void onConversationThreadContactsChanged(hookflash::core::IConversationThreadPtr conversationThread);
      virtual void onConversationThreadContactStateChanged(
                                                           hookflash::core::IConversationThreadPtr conversationThread,
                                                           hookflash::core::IContactPtr contact,
                                                           hookflash::core::IConversationThread::ContactStates state);

      virtual void onConversationThreadMessage(
                                               hookflash::core::IConversationThreadPtr conversationThread,
                                               const char *messageID);

      virtual void onConversationThreadMessageDeliveryStateChanged(
                                                                   hookflash::core::IConversationThreadPtr conversationThread,
                                                                   const char *messageID,
                                                                   hookflash::core::IConversationThread::MessageDeliveryStates state);

      virtual void onConversationThreadPushMessage(
                                                   hookflash::core::IConversationThreadPtr conversationThread,
                                                   const char *messageID,
                                                   hookflash::core::IContactPtr contact);

    private:
      boost::weak_ptr<Account> mParentAccount;
    };

    //-------------------------------------------------------------------------------
    class CallDelegate : public hookflash::core::ICallDelegate
    {
    public:
      CallDelegate(boost::shared_ptr<Account> parentAccount);
      virtual ~CallDelegate();

      virtual void onCallStateChanged(hookflash::core::ICallPtr call, hookflash::core::ICall::CallStates state);

    private:
      boost::weak_ptr<Account> mParentAccount;
    };
  };
};

#endif // HFBB_ACCOUNT_H
