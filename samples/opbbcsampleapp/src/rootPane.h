#ifndef ROOT_PANE_HPP
#define ROOT_PANE_HPP

#include <QObject>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>

namespace hookflash {
  namespace blackberry {

    class ApplicationUI;
    class LoginPane;

    class RootPane : public QObject
    {
        Q_OBJECT
    public:
        RootPane(ApplicationUI* appUI);
        virtual ~RootPane() {}

        Q_INVOKABLE void OnLoginClick(QObject* page);
        Q_INVOKABLE void OnOnLoadingChanged(int status, QString url);

        Q_INVOKABLE void OnMediaTestButton1Click();
        Q_INVOKABLE void OnMediaTestButton2Click();

        ApplicationUI* GetApplicationUI() { return mAppUI; }
        bb::cascades::QmlDocument* GetQmlDocument() { return mQml; }
    private:
        ApplicationUI* mAppUI;
        LoginPane* mLoginPane;
        bb::cascades::QmlDocument* mQml;
    };
  };
};

#endif // ROOT_PANE_HPP
