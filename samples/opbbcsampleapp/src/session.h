#ifndef HFBB_SESSION_H
#define HFBB_SESSION_H

#include "hookflash/core/types.h"
#include "hookflash/core/IStack.h"
#include "hookflash/core/IMediaEngine.h"

namespace hookflash {
  namespace blackberry {

    class SessionStackDelegate;
    class SessionMediaEngineDelegate;
    class UserIdentity;
    class Account;
    class ContactsManager;
    class ApplicationUI;

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class Session
    {
    public:
      static boost::shared_ptr<Session> CreateInstance(ApplicationUI* appUI);

      virtual ~Session() {}

      boost::shared_ptr<UserIdentity> GetIdentity() { return mIdentity; }
      boost::shared_ptr<Account> GetAccount() { return mAccount; }
      boost::shared_ptr<ContactsManager> GetContactsManager() { return mContactsManager; }
      std::string GetIdentityURI() { return mIdentityURI; }
      std::string GetPeerContactServiceDomain() { return mPeerContactServiceDomain; }
      std::string GetContactsURL() { return mContactsURL; }
      ApplicationUI* GetAppUI() { return mAppUI; }

    private:
      Session(ApplicationUI* appUI);
      void Initialize();

      boost::weak_ptr<Session> mWeakThis;
      ApplicationUI* mAppUI;
      hookflash::core::IStackPtr mStack;
      boost::shared_ptr<SessionStackDelegate> mStackDelegate;
      boost::shared_ptr<SessionMediaEngineDelegate> mMediaEngineDelegate;
      boost::shared_ptr<UserIdentity> mIdentity;
      boost::shared_ptr<Account> mAccount;
      boost::shared_ptr<ContactsManager> mContactsManager;
      std::string mIdentityURI;
      std::string mPeerContactServiceDomain;
      std::string mContactsURL;
    };

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class SessionStackDelegate : public hookflash::core::IStackDelegate
    {
    public:
      SessionStackDelegate(boost::shared_ptr<Session> parent) : mWeakParent(parent) {}

      virtual void onStackShutdown(hookflash::core::IStackAutoCleanupPtr ignoreThisArgument) {}

    private:
      boost::weak_ptr<Session> mWeakParent;
    };

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class SessionMediaEngineDelegate : public hookflash::core::IMediaEngineDelegate
    {
    public:
      SessionMediaEngineDelegate(boost::shared_ptr<Session> parent) : mWeakParent(parent) {}

      virtual void onMediaEngineAudioRouteChanged(OutputAudioRoutes audioRoute) {}
      virtual void onMediaEngineFaceDetected() {}
      virtual void onMediaEngineVideoCaptureRecordStopped() {}

    private:
      boost::weak_ptr<Session> mWeakParent;
    };

  };
};

#endif // HFBB_SESSION_H
