/*
 * QtCrossThreadNotifier.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: lawrence.gunn
 */

#include "QtCrossThreadNotifier.h"

boost::shared_ptr<zsLib::IQtCrossThreadNotifier> zsLib::IQtCrossThreadNotifier::createNotifier()
{
  return boost::shared_ptr<zsLib::IQtCrossThreadNotifier>(new QtCrossThreadNotifier);
}

QtCrossThreadNotifier::QtCrossThreadNotifier()
{

}

QtCrossThreadNotifier::~QtCrossThreadNotifier()
{
}

void QtCrossThreadNotifier::setDelegate(boost::shared_ptr<zsLib::IQtCrossThreadNotifierDelegate> delegate)
{
  if(!mDelegate) {
    mDelegate = delegate;
    connect(this, SIGNAL(signalOnMessageAvailable()), this, SLOT(onMessageAvailable())); //, Qt::QueuedConnection);
  }
}

void QtCrossThreadNotifier::notifyMessagePosted()
{
  emit signalOnMessageAvailable();
}

void QtCrossThreadNotifier::onMessageAvailable()
{
  if(mDelegate) {
    mDelegate->processMessageFromThread();
  }
}
