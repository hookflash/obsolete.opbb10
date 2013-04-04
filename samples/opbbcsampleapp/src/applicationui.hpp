#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/Application>

namespace hookflash {
  namespace blackberry {

    class RootPane;

    class ApplicationUI : public QObject
    {
        Q_OBJECT
    public:
        ApplicationUI(bb::cascades::Application* app);
        virtual ~ApplicationUI() {}

        bb::cascades::Application* GetApplication() { return mApp; }
    private:
        //-------------------------------------------------------------------------
        bb::cascades::Application* mApp;
        RootPane* mRootPane;
    };
  };
};

#endif // ApplicationUI_HPP_
