// Tabbed pane project template
#include "applicationui.hpp"

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>

// include JS Debugger / CS Profiler enabler
// this feature is enabled by default in the debug build only
#include <Qt/qdeclarativedebug.h>

using namespace bb::cascades;
using namespace hookflash::blackberry;

Q_DECL_EXPORT int main(int argc, char **argv)
{
  // this is where the server is started etc
  Application app(argc, argv);

  // localization support
  QTranslator translator;
  QString locale_string = QLocale().name();
  QString filename = QString( "opbbcsampleapp_%1" ).arg( locale_string );
  if (translator.load(filename, "app/native/qm")) {
    app.installTranslator( &translator );
  }

  // create the application pane object to init UI etc.
  new ApplicationUI(&app);

  // we complete the transaction started in the app constructor and start the client event loop here
  return Application::exec();
  // when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
