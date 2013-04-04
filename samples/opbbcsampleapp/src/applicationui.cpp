#include "applicationui.hpp"
#include "rootPane.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;
using namespace hookflash::blackberry;

ApplicationUI::ApplicationUI(Application *app) : QObject(app), mApp(app)
{
    mRootPane = new RootPane(this);
}
