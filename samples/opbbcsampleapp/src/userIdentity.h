#ifndef HFBB_IDENTITY_H
#define HFBB_IDENTITY_H

#include "hookflash/core/IIdentity.h"

namespace hookflash {
  namespace blackberry {

    class UserIdentityDelegate;
    class Session;

    class ILoginUIDelegate {
    public:
      virtual ~ILoginUIDelegate() {}
      virtual void NavigateTo(const std::string& url) = 0;
      virtual void CallJavaScript(const std::string& js) = 0;
    };

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class UserIdentity
    {
    public:
      static boost::shared_ptr<UserIdentity> CreateInstance(boost::shared_ptr<Session> session);

      virtual ~UserIdentity();

      virtual void BeginLogin(const std::string& identityURI);
      std::string GetRedirectAfterLoginCompleteURL() { return mRedirectAfterLoginCompleteURL; }
      void OnNotifyClient(const std::string& data);
      boost::shared_ptr<hookflash::core::IIdentity> GetCoreIdentity() { return mOpIdentity; }

      // Called by account object to start import of contacts.
      void OnAccountStateReady();

      // Called by ILoginUIDelegate...
      void SetLoginUIDelegate(boost::shared_ptr<ILoginUIDelegate> delegate) { mLoginUIDelegate = delegate; }
      bool OnWebBrowserPageNavigation(const std::string& url);
      void OnWaitingToMakeBrowserWindowVisible();
      void OnWaitingAssociation();

      // Called by IdentityDelegate...
      void OnWaitingToLoadBrowserWindow();
      void OnMessageForInnerBrowserWindowFrame(const std::string& message);

    private:
      UserIdentity(boost::shared_ptr<Session> session);

      boost::weak_ptr<UserIdentity> mWeakThis;
      boost::shared_ptr<Session> mSession;
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
