#include "joy_c.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Joy_C j;
    j.start();

    return a.exec();

//    ros::init(argc, argv, "joy_node");
//    Joy_C j;
//    j.fun();
//    return 0;
}
