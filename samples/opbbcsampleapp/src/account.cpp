#include "account.h"

using namespace hookflash::blackberry;

boost::shared_ptr<Account> Account::CreateInstance(boost::shared_ptr<Session> session)
{
  boost::shared_ptr<Account> instance(new Account(session));
  instance->mWeakThis = instance;
  return instance;
}

Account::Account(boost::shared_ptr<Session> session) : mSession(session)
{
}

void Account::Login()
{

}

