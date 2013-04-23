// Default empty project template
#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>
#include "applicationui.h"

#include "test/boost_replacement.h"

#include <zsLib/Log.h>

// include JS Debugger / CS Profiler enabler
// this feature is enabled by default in the debug build only
#include <Qt/qdeclarativedebug.h>

namespace hookflash { namespace services { namespace test { ZS_IMPLEMENT_SUBSYSTEM(hookflash_services_test) } } }

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
  std::cout << "TEST NOW STARTING...\n\n";

  BoostReplacement::runAllTests();
  BoostReplacement::output();

  if (0 != BoostReplacement::getGlobalFailedVar()) {
    return -1;
  }

  // this is where the server is started etc
  Application app(argc, argv);

  // localization support
  QTranslator translator;
  QString locale_string = QLocale().name();
  QString filename = QString( "hfservicesTest_%1" ).arg( locale_string );
  if (translator.load(filename, "app/native/qm")) {
      app.installTranslator( &translator );
  }

  new ApplicationUI(&app);

  // we complete the transaction started in the app constructor and start the client event loop here
  return Application::exec();
  // when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
