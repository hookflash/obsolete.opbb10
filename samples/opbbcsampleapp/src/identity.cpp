#include "Identity.h"

using namespace hookflash::blackberry;
using namespace hookflash::core;

  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------

Identity::Identity()
{
}

  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------
  //-------------------------------------------------------------------------

IdentityDelegate::IdentityDelegate(boost::shared_ptr<Identity> parentIdentity) : mParentIdentity(parentIdentity)
{
}

void IdentityDelegate::onIdentityStateChanged(IIdentityPtr identity, IdentityStates state)
{
}

void IdentityDelegate::onIdentityPendingMessageForInnerBrowserWindowFrame(IIdentityPtr identity)
{
}


