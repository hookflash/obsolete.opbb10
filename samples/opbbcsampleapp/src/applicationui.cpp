#include "applicationui.h"
#include "rootPane.h"
#include "session.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;
using namespace hookflash::blackberry;

ApplicationUI::ApplicationUI(Application *app) : QObject(app), mApp(app)
{
  mSession = Session::CreateInstance();
  mRootPane = new RootPane(this);
}
