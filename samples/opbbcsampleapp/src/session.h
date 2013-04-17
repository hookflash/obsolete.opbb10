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

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class Session
    {
    public:
      static boost::shared_ptr<Session> CreateInstance();

      virtual ~Session() {}

      boost::shared_ptr<UserIdentity> GetIdentity() { return mIdentity; }
      std::string GetIdentityURI() { return mIdentityURI; }

    private:
        Session();
        void Initialize();

        boost::weak_ptr<Session> mWeakThis;
        hookflash::core::IStackPtr mStack;
        boost::shared_ptr<SessionStackDelegate> mStackDelegate;
        boost::shared_ptr<SessionMediaEngineDelegate> mMediaEngineDelegate;
        boost::shared_ptr<UserIdentity> mIdentity;
        std::string mIdentityURI;
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
