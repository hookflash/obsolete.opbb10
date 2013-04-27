#ifndef HFBB_IDENTITY_H
#define HFBB_IDENTITY_H

#include "hookflash/core/IIdentity.h"

namespace hookflash {
  namespace blackberry {

    class UserIdentityDelegate;

    class ILoginUIDelegate {
    public:
      virtual ~ILoginUIDelegate() {}
      virtual void CallJavaScript(const std::string& js) = 0;
    };

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

      // Called by ILoginUIDelegate...
      void SetLoginUIDelegate(boost::shared_ptr<ILoginUIDelegate> delegate) { mLoginUIDelegate = delegate; }
      void OnWebBrowserPageLoaded(const std::string& url);

      // Called by IdentityDelegate...
      void OnWaitingToLoadBrowserWindow();

    private:
      UserIdentity();

      boost::weak_ptr<UserIdentity> mWeakThis;
      boost::shared_ptr<hookflash::core::IIdentity> mOpIdentity;
      boost::shared_ptr<UserIdentityDelegate> mDelegate;
      boost::shared_ptr<ILoginUIDelegate> mLoginUIDelegate;
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
