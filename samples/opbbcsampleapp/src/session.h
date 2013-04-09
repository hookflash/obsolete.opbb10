#ifndef HFBB_SESSION_H
#define HFBB_SESSION_H

#include "boost/smart_ptr.hpp"

namespace hookflash {
  namespace blackberry {

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
        boost::shared_ptr<UserIdentity> mIdentity;
        std::string mIdentityURI;
    };

  };
};

#endif // HFBB_SESSION_H
