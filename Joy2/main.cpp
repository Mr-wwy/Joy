#include "joy2.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "joy2_node");
    Joy j;
    j.fun();
    return 0;
}
