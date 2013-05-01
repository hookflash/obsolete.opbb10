#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/Application>
#include "boost/smart_ptr.hpp"
#include <zsLib/Log.h>

namespace hookflash {
  namespace blackberry {

    class RootPane;
    class Session;
    class LogDelegate;

    //-------------------------------------------------------------------------

    class ApplicationUI : public QObject
    {
        Q_OBJECT
    public:
        ApplicationUI(bb::cascades::Application* app);
        virtual ~ApplicationUI() {}

        bb::cascades::Application* GetApplication() { return mApp; }
        boost::shared_ptr<Session> GetSession() { return mSession; }
        RootPane* GetRootPane() { return mRootPane; }

    private:
        //-------------------------------------------------------------------------
        bb::cascades::Application* mApp;
        RootPane* mRootPane;
        boost::shared_ptr<Session> mSession;
        boost::shared_ptr<LogDelegate> mLog;
    };

    //-------------------------------------------------------------------------

    class LogDelegate : public zsLib::ILogDelegate {
    public:
      LogDelegate();
      virtual void onNewSubsystem(zsLib::Subsystem &inSubsystem);
      virtual void log(
                       const zsLib::Subsystem &inSubsystem,
                       zsLib::Log::Severity inSeverity,
                       zsLib::Log::Level inLevel,
                       zsLib::CSTR inMessage,
                       zsLib::CSTR inFunction,
                       zsLib::CSTR inFilePath,
                       zsLib::ULONG inLineNumber);
    };
  };
};

#endif // ApplicationUI_HPP_
