#include "joy.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "joy_node");
    Joy j;
    j.fun();
    return 0;
}
