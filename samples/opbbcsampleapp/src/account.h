#ifndef HFBB_ACCOUNT_H
#define HFBB_ACCOUNT_H

#include <hookflash/core/IAccount.h>
#include <hookflash/core/IConversationThread.h>
#include <hookflash/core/ICall.h>

namespace hookflash {
  namespace blackberry {

    class Account;
    typedef boost::shared_ptr<Account> AccountPtr;
    typedef boost::weak_ptr<Account> AccountWeakPtr;

    class Session;
    typedef boost::shared_ptr<Session> SessionPtr;
    typedef boost::weak_ptr<Session> SessionWeakPtr;

    class Contact;
    typedef boost::shared_ptr<Contact> ContactPtr;
    typedef boost::weak_ptr<Contact> ContactWeakPtr;

    //-------------------------------------------------------------------------------
    class Account : public hookflash::core::IAccountDelegate,
                    public hookflash::core::IConversationThreadDelegate,
                    public hookflash::core::ICallDelegate
    {
    public:
      typedef zsLib::PUID PUID;
      typedef zsLib::String String;

    public:
      static AccountPtr CreateInstance(SessionPtr session);

      Account(SessionPtr session);
      virtual ~Account();

      void Login();
      bool Relogin(const std::string& peerFile, const std::string& secret);

      void ProcessMyFBProfile(const std::string& data);
      void ProcessFbFriends(const std::string& data);
      boost::shared_ptr<Session> GetSession() { return mSession; }

      std::string ReadPrivatePeerFile();
      std::string ReadPrivatePeerSecretFile();
      void WritePeerFiles();

      hookflash::core::IAccountPtr GetCoreAccount() { return mOpAccount; }
      hookflash::core::ICallPtr GetCoreCall() { return mCall; }

      bool PlaceCallByFacebookID(
          const char *facebookID,
          bool includeAudio,
          bool includeVideo
          );

      ContactPtr GetActiveCallContact();  // returns ContractPtr() if no party is active

    protected:
      // IAccountDelegate
      virtual void onAccountStateChanged(hookflash::core::IAccountPtr account, AccountStates state);
      virtual void onAccountAssociatedIdentitiesChanged(hookflash::core::IAccountPtr account);

      // IConversationTheadDelegate
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

      // ICallDelegate
      virtual void onCallStateChanged(hookflash::core::ICallPtr call, hookflash::core::ICall::CallStates state);

    protected:
      String log(const char *message) const;

    private:
      PUID mID;
      AccountWeakPtr mThisWeak;
      SessionPtr mSession;
      hookflash::core::IAccountPtr mOpAccount;

      hookflash::core::ICallPtr mCall;
    };

  };
};

#endif // HFBB_ACCOUNT_H
