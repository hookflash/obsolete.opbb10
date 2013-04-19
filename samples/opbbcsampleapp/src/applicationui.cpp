#include "applicationui.h"
#include "rootPane.h"
#include "session.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

// TESTING
#include "zsLib/RegEx.h"

using namespace bb::cascades;
using namespace hookflash::blackberry;

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

ApplicationUI::ApplicationUI(Application *app) : QObject(app), mApp(app)
{
  // Add a log listener and pipe the output through qDebug.
  mLog = boost::shared_ptr<LogDelegate>(new LogDelegate());
  zsLib::Log::singleton()->addListener(mLog);

  mSession = Session::CreateInstance();
  mRootPane = new RootPane(this);
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

LogDelegate::LogDelegate()
{
}

void LogDelegate::onNewSubsystem(zsLib::Subsystem &inSubsystem)
{
  inSubsystem.setOutputLevel(zsLib::Log::Debug);
}

void LogDelegate::log(
                       const zsLib::Subsystem &inSubsystem,
                       zsLib::Log::Severity inSeverity,
                       zsLib::Log::Level inLevel,
                       zsLib::CSTR inMessage,
                       zsLib::CSTR inFunction,
                       zsLib::CSTR inFilePath,
                       zsLib::ULONG inLineNumber)
{
  qDebug() << "OP[" << inSubsystem.getName() << ":" << (int) inSeverity << "] " << inMessage << " - " << inFunction << ":" << inFilePath << " at " << inLineNumber;
}
