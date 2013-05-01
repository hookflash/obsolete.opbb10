#ifndef HFBB_IDENTITY_H
#define HFBB_IDENTITY_H

#include "hookflash/core/IIdentity.h"

namespace hookflash {
  namespace blackberry {

    class ILoginUIDelegate;
    typedef boost::shared_ptr<ILoginUIDelegate> ILoginUIDelegatePtr;
    typedef boost::weak_ptr<ILoginUIDelegate> ILoginUIDelegateWeakPtr;

    class Session;
    typedef boost::shared_ptr<Session> SessionPtr;
    typedef boost::weak_ptr<Session> SessionWeakPtr;

    class UserIdentity;
    typedef boost::shared_ptr<UserIdentity> UserIdentityPtr;
    typedef boost::weak_ptr<UserIdentity> UserIdentityWeakPtr;

    class ILoginUIDelegate {
    public:
      virtual ~ILoginUIDelegate() {}
      virtual void NavigateTo(const std::string& url) = 0;
      virtual void CallJavaScript(const std::string& js) = 0;
    };

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    class UserIdentity : public hookflash::core::IIdentityDelegate
    {
    public:
      static UserIdentityPtr CreateInstance(SessionPtr session);

      virtual ~UserIdentity();

      virtual void BeginLogin(const std::string& identityURI);
      std::string GetRedirectAfterLoginCompleteURL() { return mRedirectAfterLoginCompleteURL; }
      void OnNotifyClient(const std::string& data);
      hookflash::core::IIdentityPtr GetCoreIdentity() { return mOpIdentity; }

      // Called by ILoginUIDelegate...
      void SetLoginUIDelegate(boost::shared_ptr<ILoginUIDelegate> delegate) { mLoginUIDelegate = delegate; }
      bool OnWebBrowserPageNavigation(const std::string& url);
      void OnWaitingToMakeBrowserWindowVisible();
      void OnWaitingAssociation();

      // Called by IdentityDelegate...
      void OnWaitingToLoadBrowserWindow();
      void OnMessageForInnerBrowserWindowFrame(const std::string& message);

      // hookflash::core::IIdentityDelegate
      virtual void onIdentityStateChanged(hookflash::core::IIdentityPtr identity, IdentityStates state);
      virtual void onIdentityPendingMessageForInnerBrowserWindowFrame(hookflash::core::IIdentityPtr identity);

    private:
      UserIdentity(boost::shared_ptr<Session> session);

      UserIdentityWeakPtr mThisWeak;
      SessionPtr mSession;
      hookflash::core::IIdentityPtr mOpIdentity;
      ILoginUIDelegatePtr mLoginUIDelegate;
      std::string mRedirectAfterLoginCompleteURL;
    };
  };
};

#endif // HFBB_IDENTITY_H
