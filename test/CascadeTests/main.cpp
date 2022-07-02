#include "tst_node.h"
#include "tst_nodegraphdatamodel.h"
#include "tst_nodegraphview.h"
#include "tst_slider.h"

#include <QApplication>

#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
