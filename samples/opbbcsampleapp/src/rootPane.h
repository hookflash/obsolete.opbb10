#ifndef ROOT_PANE_HPP
#define ROOT_PANE_HPP

#include <QObject>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/ListView>
#include <bb/cascades/GroupDataModel>
#include <hookflash/core/IIdentityLookup.h>

#include "video_render_bb_impl.h"
#include "bb_window_wrapper.h"
#include "video_render.h"
#include "boost/smart_ptr.hpp"

namespace hookflash {
  namespace blackberry {

    class ApplicationUI;
    class LoginPane;
    class RootPaneIdentityLookupDelegate;
    class ContactsManager;

    class RootPane : public QObject
    {
        Q_OBJECT
    public:
        RootPane(ApplicationUI* appUI);
        virtual ~RootPane() {}

        Q_INVOKABLE void OnLoginClick(QObject* page);
        Q_INVOKABLE void OnOnLoadingChanged(int status, QString url);

        Q_INVOKABLE void OnCallWindowOpened(QObject* callPageObj);

        Q_INVOKABLE void OnMediaTestButton1Click();
        Q_INVOKABLE void OnMediaTestButton2Click();

        void ProcessFbFriends(const QString& data);
        void AddContactsToUI(hookflash::core::IIdentityLookupPtr lookup);

        ApplicationUI* GetApplicationUI() { return mAppUI; }
        bb::cascades::QmlDocument* GetQmlDocument() { return mQml; }
        LoginPane* GetLoginPane() { return mLoginPane; }

    public Q_SLOTS:
        void onLayoutFrameChanged(const QRectF &layoutFrame);

    private:
        void CreateVideoRenderer();

        ApplicationUI* mAppUI;
        LoginPane* mLoginPane;
        bb::cascades::QmlDocument* mQml;
        bb::cascades::ForeignWindowControl* mForeignWindow;
        bb::cascades::ListView* mContactsListView;
        boost::shared_ptr<webrtc::BlackberryWindowWrapper> mVideoRenderer;
        QRectF mVideoWindowSize;
        bool mCallWindowIsOpen;
        bb::cascades::GroupDataModel* mContactModel;
        hookflash::core::IIdentityLookupPtr mIdentityLookup;
        boost::shared_ptr<RootPaneIdentityLookupDelegate> mIdentityLookupDelegate;
    };

    class RootPaneIdentityLookupDelegate : public hookflash::core::IIdentityLookupDelegate
    {
    public:
      RootPaneIdentityLookupDelegate(RootPane* rootPane) : mRootPane(rootPane) {}
      virtual ~RootPaneIdentityLookupDelegate() {}

      virtual void onIdentityLookupCompleted(hookflash::core::IIdentityLookupPtr lookup);

    private:
      RootPane* mRootPane; // Bare pointer - this is owned by QT
    };

  };
};

#endif // ROOT_PANE_HPP
