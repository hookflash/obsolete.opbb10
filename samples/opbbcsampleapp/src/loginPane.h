#ifndef LOGIN_PANE_HPP
#define LOGIN_PANE_HPP

#include <QObject>
#include <bb/cascades/Page>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/WebView>
#include "boost/smart_ptr.hpp"

namespace hookflash {
  namespace blackberry {

    class RootPane;
    class Session;
    class Identity;

    class LoginPane : public QObject
    {
        Q_OBJECT
    public:
        LoginPane(boost::shared_ptr<Session> session, RootPane* rootPane, bb::cascades::NavigationPane* navigationPane);
        virtual ~LoginPane() {}

        Q_INVOKABLE void OnLoginClick();
        Q_INVOKABLE void OnLoadingChanged(int status, QUrl url);
        Q_INVOKABLE void TestCallback();
    private:
        RootPane* mRootPane;
        bb::cascades::WebView* mWebView;
        boost::shared_ptr<Identity> mIdentity;
    };
  };
};

#endif // LOGIN_PANE_HPP
