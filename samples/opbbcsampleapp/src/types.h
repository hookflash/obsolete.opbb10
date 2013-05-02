
#pragma once

#ifndef HOOKFLASH_BLACKBERRY_TYPES_H
#define HOOKFLASH_BLACKBERRY_TYPES_H

#include <hookflash/core/types.h>
#include <zsLib/Log.h>
#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace hookflash
{
  namespace blackberry
  {

    // non boost class forwards
    class ApplicationUI;
    class LoginPane;
    class LogDelegate;
    class RootPane;

    // boost based class forwards
    class Account;
    typedef boost::shared_ptr<Account> AccountPtr;
    typedef boost::weak_ptr<Account> AccountWeakPtr;

    class Contact;
    typedef boost::shared_ptr<Contact> ContactPtr;
    typedef boost::weak_ptr<Contact> ContactWeakPtr;

    class ContactsManager;
    typedef boost::shared_ptr<ContactsManager> ContactsManagerPtr;
    typedef boost::weak_ptr<ContactsManager> ContactsManagerWeakPtr;

    class LoginPaneLoginUIDelegate;
    typedef boost::shared_ptr<LoginPaneLoginUIDelegate> LoginPaneLoginUIDelegatePtr;
    typedef boost::weak_ptr<LoginPaneLoginUIDelegate> LoginPaneLoginUIDelegateWeakPtr;

    class ILoginUIDelegate;
    typedef boost::shared_ptr<ILoginUIDelegate> ILoginUIDelegatePtr;
    typedef boost::weak_ptr<ILoginUIDelegate> ILoginUIDelegateWeakPtr;

    class RootPaneCallback;
    typedef boost::shared_ptr<RootPaneCallback> RootPaneCallbackPtr;
    typedef boost::weak_ptr<RootPaneCallback> RootPaneCallbackWeakPtr;

    class Session;
    typedef boost::shared_ptr<Session> SessionPtr;
    typedef boost::weak_ptr<Session> SessionWeakPtr;

    class UserIdentity;
    typedef boost::shared_ptr<UserIdentity> UserIdentityPtr;
    typedef boost::weak_ptr<UserIdentity> UserIdentityWeakPtr;

  }
}

#endif //HOOKFLASH_BLACKBERRY_TYPES_H
