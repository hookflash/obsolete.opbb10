#ifndef LOGIN_PANE_HPP
#define LOGIN_PANE_HPP

#include <QObject>
#include <bb/cascades/Page>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/WebView>
#include <userIdentity.h>

#include "types.h"

namespace hookflash {
  namespace blackberry {

    class LoginPane : public QObject, public ILoginUIDelegate
    {
        Q_OBJECT
    public:
        friend class LoginPaneDelegates;

    public:
        LoginPane(boost::shared_ptr<Session> session, RootPane* rootPane);
        virtual ~LoginPane();

        Q_INVOKABLE void OnLoginClick(bb::cascades::NavigationPane* navigationPane);
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
        SessionPtr mSession;
        UserIdentityPtr mIdentity;
        LoginPaneDelegatesPtr mThisDelegates;
        bool mPageHasLoaded;
        std::string mJsToEvaluateWhenPageLoaded;
    };

    class LoginPaneDelegates : public ILoginUIDelegate
    {
    public:
      LoginPaneDelegates(LoginPane* outer) : mOuter(outer) {}
      virtual ~LoginPaneDelegates() {}

      void destroy() {mOuter = NULL;}

      virtual void NavigateTo(const std::string& url) { if (!mOuter) return; mOuter->NavigateTo(url); }
      virtual void CallJavaScript(const std::string& js) { if (!mOuter) return; mOuter->CallJavaScript(js); }

    private:
      LoginPane *mOuter;
    };
  };
};

#endif // LOGIN_PANE_HPP
