#include "applicationui.h"
#include "rootPane.h"
#include "session.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <hookflash/core/ILogger.h>

#include <zsLib/Log.h>

//#include <hookflash/core/internal/core_MediaEngine.h>

namespace hookflash { namespace blackberry { ZS_DECLARE_SUBSYSTEM(hookflash_blackberry) } }

using namespace bb::cascades;
using namespace hookflash::blackberry;
using hookflash::core::ILogger;

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

ApplicationUI::ApplicationUI(Application *app) : QObject(app), mApp(app)
{
  // Add a log listener and pipe the output through qDebug.
//  mLog = boost::shared_ptr<LogDelegate>(new LogDelegate());
//  zsLib::Log::singleton()->addListener(mLog);
  ILogger::setLogLevel(ILogger::Basic);
  ILogger::setLogLevel("hookflash_webrtc", ILogger::Detail);
  ILogger::installTelnetLogger(59999, 60, true);
  ILogger::installDebuggerLogger();

  mSession = Session::CreateInstance(this);
  mRootPane = new RootPane(this);
//
//	  hookflash::core::IMediaEnginePtr mediaEngine = hookflash::core::IMediaEngine::singleton();
//	  mediaEngine->startVideoCapture();
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
