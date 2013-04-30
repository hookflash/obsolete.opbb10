#ifndef LOGIN_PANE_HPP
#define LOGIN_PANE_HPP

#include <QObject>
#include <bb/cascades/Page>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/WebView>
#include "boost/smart_ptr.hpp"
#include <userIdentity.h>

namespace hookflash {
  namespace blackberry {

    class RootPane;
    class Session;
    class UserIdentity;
    class LoginPaneLoginUIDelegate;

    class LoginPane : public QObject, public ILoginUIDelegate
    {
        Q_OBJECT
    public:
        LoginPane(boost::shared_ptr<Session> session, RootPane* rootPane, bb::cascades::NavigationPane* navigationPane);
        virtual ~LoginPane() {}

        Q_INVOKABLE void OnLoginClick();
        Q_INVOKABLE bool OnNavigationRequested(QUrl url);
        Q_INVOKABLE void OnLoadingChanged(int status, QUrl url);
        Q_INVOKABLE void TestCallback();

        void NavigateTo(const std::string& url);
        void CallJavaScript(const std::string& js);

    private:
        void CallJavaScriptAfterPageLoad();

        RootPane* mRootPane;
        bb::cascades::Page* mPage;
        bb::cascades::WebView* mWebView;
        boost::shared_ptr<Session> mSession;
        boost::shared_ptr<UserIdentity> mIdentity;
        boost::shared_ptr<LoginPaneLoginUIDelegate> mLoginUIDelegate;
        bool mPageHasLoaded;
        std::string mJsToEvaluateWhenPageLoaded;
    };

    class LoginPaneLoginUIDelegate : public ILoginUIDelegate
    {
    public:
        LoginPaneLoginUIDelegate(LoginPane* parentPane) : mParentPane(parentPane) {}
        virtual ~LoginPaneLoginUIDelegate() {}

        void NavigateTo(const std::string& url) { mParentPane->NavigateTo(url); }
        virtual void CallJavaScript(const std::string& js) { mParentPane->CallJavaScript(js); }

    private:
        LoginPane* mParentPane;
    };
  };
};

#endif // LOGIN_PANE_HPP
