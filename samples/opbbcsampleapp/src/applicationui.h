#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/Application>
#include "boost/smart_ptr.hpp"

namespace hookflash {
  namespace blackberry {

    class RootPane;
    class Session;

    class ApplicationUI : public QObject
    {
        Q_OBJECT
    public:
        ApplicationUI(bb::cascades::Application* app);
        virtual ~ApplicationUI() {}

        bb::cascades::Application* GetApplication() { return mApp; }
        boost::shared_ptr<Session> GetSession() { return mSession; }
    private:
        //-------------------------------------------------------------------------
        bb::cascades::Application* mApp;
        RootPane* mRootPane;
        boost::shared_ptr<Session> mSession;
    };
  };
};

#endif // ApplicationUI_HPP_
