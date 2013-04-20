/*
 *  Created by Robin Raymond.
 *  Copyright 2009-2013. Robin Raymond. All rights reserved.
 *
 * This file is part of zsLib.
 *
 * zsLib is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License (LGPL) as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * zsLib is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with zsLib; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */

#ifndef BOOST_AUTO_TEST_SUITE

#include <zsLib/types.h>
#include <iostream>
#include <QDebug>

namespace BoostReplacement
{
  zsLib::ULONG &getGlobalPassedVar();
  zsLib::ULONG &getGlobalFailedVar();

  void passed();
  void failed();
  void output();
}

#define BOOST_AUTO_TEST_SUITE(xParam) namespace xParam {

#define BOOST_AUTO_TEST_SUITE_END() }

#define BOOST_AUTO_TEST_CASE(xTestCase) static struct Test_##xTestCase {   \
    Test_##xTestCase()                                              \
    {                                                               \
      qDebug() << "STARTING:     " #xTestCase "\n";                \
      try                                                           \
      { test_func(); }                                              \
      catch(...)                                                    \
      { qDebug() << "***UNCAUGHT EXCEPTION IN***: " #xTestCase "\n"; BoostReplacement::failed(); }   \
      qDebug() << "ENDING:       " #xTestCase "\n\n";              \
    }                                                               \
    void test_func();                                               \
  } g_Test_##xTestCase;                                             \
\
  void Test_##xTestCase::test_func()

#define BOOST_CHECK(xValue)                                           \
  {                                                                   \
    if (!(xValue))                                                    \
    { qDebug() << "***FAILED***: " #xValue "\n"; BoostReplacement::failed(); } \
    else                                                                        \
    { qDebug() << "PASSED:       " #xValue "\n"; BoostReplacement::passed(); } \
  }

#define BOOST_EQUAL(xValue1, xValue2)                               \
  if (!((xValue1) == (xValue2)))                                    \
  { qDebug() << "***FAILED***: " #xValue1 " == " #xValue2 ", V1=" << (xValue1) << ", V2=" << (xValue2) << "\n"; BoostReplacement::failed(); }                   \
  else                                                              \
  { qDebug() << "PASSED:       " #xValue1 " == " #xValue2 ", V1=" << (xValue1) << ", V2=" << (xValue2) << "\n"; BoostReplacement::passed(); }

#endif //BOOST_AUTO_TEST_SUITE
