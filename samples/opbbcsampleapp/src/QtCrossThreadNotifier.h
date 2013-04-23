/*
 * QtCrossThreadNotifier.h
 *
 *  Created on: Apr 21, 2013
 *      Author: lawrence.gunn
 */

#ifndef QTCROSSTHREADNOTIFIER_H_
#define QTCROSSTHREADNOTIFIER_H_

#include <zsLib/MessageQueueThread.h>
#include <QObject>

class QtCrossThreadNotifier : public QObject, public zsLib::IQtCrossThreadNotifier
{
    Q_OBJECT
public:
  QtCrossThreadNotifier();
  virtual ~QtCrossThreadNotifier();

  virtual void setDelegate(boost::shared_ptr<zsLib::IQtCrossThreadNotifierDelegate> delegate);
  virtual void notifyMessagePosted();


public slots:
  void onMessageAvailable();

signals:
  void signalOnMessageAvailable();

private:
  boost::shared_ptr<zsLib::IQtCrossThreadNotifierDelegate> mDelegate;
};

#endif // QTCROSSTHREADNOTIFIER_H_
