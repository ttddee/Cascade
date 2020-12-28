#include "tst_nodebasetests.h"
#include "tst_cssliderboxtests.h"
#include "tst_resizepropertiesentitytests.h"

#include <QApplication>

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
