#ifndef HFBB_SESSION_H
#define HFBB_SESSION_H

#include <hookflash/core/types.h>
#include <hookflash/core/IStack.h>
#include <hookflash/core/IMediaEngine.h>

#include "types.h"

namespace hookflash {
  namespace blackberry {

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class Session : public hookflash::core::IStackDelegate,
                    public hookflash::core::IMediaEngineDelegate
    {
    public:
      static SessionPtr CreateInstance(ApplicationUI* appUI);

      virtual ~Session() {}

      UserIdentityPtr GetIdentity() { return mIdentity; }
      AccountPtr GetAccount() { return mAccount; }
      ContactsManagerPtr GetContactsManager() { return mContactsManager; }

      std::string GetIdentityURI() { return mIdentityURI; }
      std::string GetPeerContactServiceDomain() { return mPeerContactServiceDomain; }
      std::string GetContactsURL() { return mContactsURL; }
      ApplicationUI* GetAppUI() { return mAppUI; }

    protected:
      // IStackDelegate
      virtual void onStackShutdown(hookflash::core::IStackAutoCleanupPtr ignoreThisArgument) {}

      // IMediaEngineDelegate
      virtual void onMediaEngineAudioRouteChanged(OutputAudioRoutes audioRoute) {}
      virtual void onMediaEngineFaceDetected() {}
      virtual void onMediaEngineVideoCaptureRecordStopped() {}

    private:
      Session(ApplicationUI* appUI);
      void Initialize();

      SessionWeakPtr mThisWeak;
      ApplicationUI* mAppUI;
      hookflash::core::IStackPtr mStack;

      UserIdentityPtr mIdentity;
      AccountPtr mAccount;
      ContactsManagerPtr mContactsManager;

      std::string mIdentityURI;
      std::string mPeerContactServiceDomain;
      std::string mContactsURL;
    };
  };
};

#endif // HFBB_SESSION_H
