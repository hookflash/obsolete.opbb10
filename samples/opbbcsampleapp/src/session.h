#ifndef HFBB_SESSION_H
#define HFBB_SESSION_H

#include "boost/smart_ptr.hpp"

namespace hookflash {
  namespace blackberry {

    class Identity;

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    class Session
    {
    public:
      static boost::shared_ptr<Session> CreateInstance();

      virtual ~Session() {}

      boost::shared_ptr<Identity> GetIdentity() { return mIdentity; }

    private:
        Session();
        void Initialize();

        boost::weak_ptr<Session> mWeakThis;
        boost::shared_ptr<Identity> mIdentity;
    };

  };
};

#endif // HFBB_SESSION_H
