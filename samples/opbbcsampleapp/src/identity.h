#ifndef HFBB_IDENTITY_H
#define HFBB_IDENTITY_H

#include "hookflash/core/IIdentity.h"

namespace hookflash {
  namespace blackberry {

    class IdentityDelegate;

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class Identity
    {
    public:
        Identity();
        virtual ~Identity() {}

    private:
        boost::shared_ptr<hookflash::core::IIdentity> mOpIdentity;
        boost::shared_ptr<IdentityDelegate> mDelegate;
    };

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class IdentityDelegate : public hookflash::core::IIdentityDelegate
    {
    public:
      IdentityDelegate(boost::shared_ptr<Identity> parentIdentity);
      virtual ~IdentityDelegate() {}

      virtual void onIdentityStateChanged(hookflash::core::IIdentityPtr identity, IdentityStates state);
      virtual void onIdentityPendingMessageForInnerBrowserWindowFrame(hookflash::core::IIdentityPtr identity);

    private:
      boost::weak_ptr<Identity> mParentIdentity;
    };

  };
};

#endif // HFBB_IDENTITY_H
