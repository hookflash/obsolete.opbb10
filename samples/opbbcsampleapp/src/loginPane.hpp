#ifndef LOGIN_PANE_HPP
#define LOGIN_PANE_HPP

#include <QObject>
#include <bb/cascades/Page>

namespace hookflash {
  namespace blackberry {

    class RootPane;

    class LoginPane : public QObject
    {
        Q_OBJECT
    public:
        LoginPane(RootPane* rootPane, bb::cascades::Page* loginPage);
        virtual ~LoginPane() {}

        Q_INVOKABLE void OnLoginClick();
        Q_INVOKABLE void OnOnLoadingChanged(int status, QString url);
    private:
        RootPane* mRootPane;
        bb::cascades::Page* mLoginPage;
    };
  };
};

#endif // LOGIN_PANE_HPP
