#ifndef HFBB_ACCOUNT_H
#define HFBB_ACCOUNT_H

#include "hookflash/core/IAccount.h"

namespace hookflash {
  namespace blackberry {

    class Session;
    class AccountDelegate;

    class Account
    {
    public:
      static boost::shared_ptr<Account> CreateInstance(boost::shared_ptr<Session> session);
      virtual ~Account() {}

      void Login();

    private:
      Account(boost::shared_ptr<Session> session);

      boost::weak_ptr<Account> mWeakThis;
      boost::shared_ptr<Session> mSession;
      boost::shared_ptr<hookflash::core::IAccount> mOpAccount;
      boost::shared_ptr<AccountDelegate> mDelegate;
    };

    class AccountDelegate : public hookflash::core::IAccountDelegate
    {
       virtual void onAccountStateChanged(hookflash::core::IAccountPtr account, AccountStates state);
       virtual void onAccountAssociatedIdentitiesChanged(hookflash::core::IAccountPtr account);
    };

  };
};

#endif // HFBB_ACCOUNT_H
