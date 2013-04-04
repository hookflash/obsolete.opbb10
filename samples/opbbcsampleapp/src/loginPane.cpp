#include "loginPane.hpp"
#include "rootPane.hpp"
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <iostream>

using namespace bb::cascades;
using namespace hookflash::blackberry;

LoginPane::LoginPane(RootPane* rootPane, Page* loginPage) : QObject(rootPane), mRootPane(rootPane), mLoginPage(loginPage)
{
  std::cerr << L"***************** LoginPane" << std::endl;
}

void LoginPane::OnLoginClick()
{
  std::cerr << L"***************** LoginPane::OnLoginClick" << std::endl;
  return;
}

void LoginPane::OnOnLoadingChanged(int status, QString url)
{

}


