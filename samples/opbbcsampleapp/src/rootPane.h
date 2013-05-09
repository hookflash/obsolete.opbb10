#ifndef ROOT_PANE_HPP
#define ROOT_PANE_HPP

#include <QObject>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>
#include <bb/cascades/WebView>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/ListView>
#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/Tab>
#include <bb/cascades/TabbedPane>
#include <bb/system/SystemDialog>

#include <hookflash/core/IIdentityLookup.h>
#include <hookflash/core/IContactPeerFilePublicLookup.h>

#include "types.h"

#include "userIdentity.h"

#include "video_render_bb_impl.h"
#include "bb_window_wrapper.h"
#include "video_render.h"


namespace hookflash {
  namespace blackberry {

    class RootPane : public QObject
    {
        Q_OBJECT
        // The messages that have been sent between local and remote peer
        Q_PROPERTY(QString chatHistory READ chatHistory NOTIFY chatHistoryChanged)
    public:
        friend class RootPaneDelegates;

    public:
        RootPane(ApplicationUI* appUI);
        virtual ~RootPane();

        // The user clicks the sign-in button.
        Q_INVOKABLE void OnPrestartWebkit();

        // The user clicks the sign-in button.
        Q_INVOKABLE void BeginLogin();

        // The login web view is about to navigate to this URL. Return true to cancel the navigation.
        Q_INVOKABLE bool OnLoginNavigationRequested(QUrl url);

        // The login loading state changed. The status is 0 for starting, 1 for success, 2 for failure.
        Q_INVOKABLE void OnLoginLoadingChanged(int status, QString url);

        // As per above, but these are for the contacts webview used for accessing contact info.
        Q_INVOKABLE bool OnContactsNavigationRequested(QUrl url);
        Q_INVOKABLE void OnContactsLoadingChanged(int status, QString url);

        // The video/voice window is opened/visible or closed/hidden
        Q_INVOKABLE void OnVideoCallWindowOpened();
        Q_INVOKABLE void OnVideoCallWindowClosed();

        Q_INVOKABLE bool SendTextMessage(QString currentTextUserId, QString text);
        Q_INVOKABLE void IncomingTextMessage(QString incomingTextUserId, QString incomingText);
        Q_INVOKABLE void updateChatWindow(const QString msg);
        Q_INVOKABLE bool StartCall(QString currentRemoteUserId, bool hasVideo);
        Q_INVOKABLE void EndCall();
        Q_INVOKABLE void OnCallEnded();
        Q_INVOKABLE void HandleCall(ContactPtr caller, ContactPtr callee, const char* state);

        void ProcessFbFriends(const QString& data);
        void AddContactsToUI();

        ApplicationUI* GetApplicationUI() { return mAppUI; }
        bb::cascades::QmlDocument* GetQmlDocument() { return mQml; }
        boost::shared_ptr<webrtc::BlackberryWindowWrapper> GetRenderWindow() { return mVideoRenderer; }

        void LoginNavigateTo(const std::string& url);
        void LoginCallJavaScript(const std::string& js);
        void LoginMakeBrowserWindowVisible();
        void LoginHideBrowserAfterLogin();

        void ContactsNavigateTo(const std::string& url);
        void ContactsCallJavaScript(const std::string& js);

        void WriteContacts(QString rawContacts);
        std::string ReadContacts();

        void LoginSuccessful();
        void LoginFailed();

        void ShowNewMessage(ContactPtr contact, const char* message);


        // The accessor method of the property
        QString chatHistory() const;

    protected:
        // IIdentityLookupDelegate
        virtual void onIdentityLookupCompleted(hookflash::core::IIdentityLookupPtr lookup);

        // IContactPeerFilePublicLookupDelegate
        virtual void onContactPeerFilePublicLookupCompleted(hookflash::core::IContactPeerFilePublicLookupPtr lookup);

    public Q_SLOTS:
        void onLayoutFrameChanged(const QRectF &layoutFrame);
        void onDialogFinished(bb::system::SystemUiResult::Type type);
    Q_SIGNALS:
		// The change notification signal of the property
		void chatHistoryChanged();

    protected:
        zsLib::String log(const char *message) const;

    private:
        void CreateVideoRenderer();
        void LoginCallJavaScriptAfterPageLoad();
        void ContactsCallJavaScriptAfterPageLoad();

        zsLib::PUID mID;

        RootPaneDelegatesPtr mThisDelegates;
        bool mLoginPageHasLoaded;
        std::string mLoginJsToEvaluateWhenPageLoaded;
        bool mContactsPageHasLoaded;
        std::string mContactsJsToEvaluateWhenPageLoaded;

        ApplicationUI* mAppUI;
        bb::cascades::QmlDocument* mQml;
        bb::cascades::AbstractPane* mRoot;
        bb::cascades::ForeignWindowControl* mForeignWindow;
        bb::cascades::WebView* mLoginWebView;
        bb::cascades::WebView* mContactsWebView;
        bb::cascades::ListView* mContactsListView;
        boost::shared_ptr<webrtc::BlackberryWindowWrapper> mVideoRenderer;
        QRectF mVideoWindowSize;
        bool mCallWindowIsOpen;
        bb::cascades::ArrayDataModel* mArrayContactsModel;
        bb::cascades::GroupDataModel* mGroupContactsModel;

        ContactPtr mRemoteCallContact;

        hookflash::core::IIdentityLookupPtr mIdentityLookup;
        hookflash::core::IContactPeerFilePublicLookupPtr mContactPeerFilePublicLookup;

        QString mChatHistory;
    };

    class RootPaneDelegates : public hookflash::core::IIdentityLookupDelegate,
                              public hookflash::core::IContactPeerFilePublicLookupDelegate,
                              public ILoginUIDelegate
    {
    public:
      RootPaneDelegates(RootPane* outer) : mOuter(outer) {}
      virtual ~RootPaneDelegates() {}

      void destroy() {mOuter = NULL;}

      // IIdentityLookupDelegate
      virtual void onIdentityLookupCompleted(hookflash::core::IIdentityLookupPtr lookup);

      // IContactPeerFilePublicLookupDelegate
      virtual void onContactPeerFilePublicLookupCompleted(hookflash::core::IContactPeerFilePublicLookupPtr lookup);

      virtual void LoginNavigateTo(const std::string& url) { if (!mOuter) return; mOuter->LoginNavigateTo(url); }
      virtual void LoginCallJavaScript(const std::string& js) { if (!mOuter) return; mOuter->LoginCallJavaScript(js); }
      virtual void LoginMakeBrowserWindowVisible() { if (!mOuter) return; mOuter->LoginMakeBrowserWindowVisible(); }
      virtual void LoginHideBrowserAfterLogin() { if (!mOuter) return; mOuter->LoginHideBrowserAfterLogin(); }

    private:
      RootPane* mOuter; // Bare pointer - this is owned by QT
    };

  };
};

#endif // ROOT_PANE_HPP
