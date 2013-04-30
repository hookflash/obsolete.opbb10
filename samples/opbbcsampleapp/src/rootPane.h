#ifndef ROOT_PANE_HPP
#define ROOT_PANE_HPP

#include <QObject>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/ListView>

#include "video_render_bb_impl.h"
#include "bb_window_wrapper.h"
#include "video_render.h"
#include "boost/smart_ptr.hpp"

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

        Q_INVOKABLE void OnCallWindowOpened(QObject* callPageObj);

        Q_INVOKABLE void OnMediaTestButton1Click();
        Q_INVOKABLE void OnMediaTestButton2Click();

        void ProcessFbFriends(const QString& data);

        ApplicationUI* GetApplicationUI() { return mAppUI; }
        bb::cascades::QmlDocument* GetQmlDocument() { return mQml; }

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
    };
  };
};

#endif // ROOT_PANE_HPP
