#ifndef HFBB_IDENTITY_H
#define HFBB_IDENTITY_H

#include "hookflash/core/IIdentity.h"

namespace hookflash {
  namespace blackberry {

    class UserIdentityDelegate;

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class UserIdentity
    {
    public:
      static boost::shared_ptr<UserIdentity> CreateInstance();

      virtual ~UserIdentity();

      virtual void BeginLogin(const std::string& identityURI);
      std::string GetRedirectAfterLoginCompleteURL() { return mRedirectAfterLoginCompleteURL; }

    private:
      UserIdentity();

      boost::weak_ptr<UserIdentity> mWeakThis;
      boost::shared_ptr<hookflash::core::IIdentity> mOpIdentity;
      boost::shared_ptr<UserIdentityDelegate> mDelegate;
      std::string mRedirectAfterLoginCompleteURL;
    };

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class UserIdentityDelegate : public hookflash::core::IIdentityDelegate
    {
    public:
      UserIdentityDelegate(boost::shared_ptr<UserIdentity> parentIdentity);
      virtual ~UserIdentityDelegate();

      virtual void onIdentityStateChanged(hookflash::core::IIdentityPtr identity, IdentityStates state);
      virtual void onIdentityPendingMessageForInnerBrowserWindowFrame(hookflash::core::IIdentityPtr identity);

    private:
      boost::weak_ptr<UserIdentity> mParentIdentity;
    };

  };
};

#endif // HFBB_IDENTITY_H
