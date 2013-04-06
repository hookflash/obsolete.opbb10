#ifndef HFBB_ACCOUNT_H
#define HFBB_ACCOUNT_H

#include "hookflash/core/IAccount.h"

namespace hookflash {
  namespace blackberry {

    class AccountDelegate;

    class Account
    {
    public:
        Account();
        virtual ~Account() {}

    private:
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
