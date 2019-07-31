#include"joy_c.h"
#include<QDebug>

Joy_C::Joy_C()
{
}

Joy_C::~Joy_C()
{
}

void Joy_C::run()
{
    // Parameter conversions
    double scale = -1. / (1. - deadzone_) / 32767.;
    double unscaled_deadzone = 32767. * deadzone_;

    js_event event;
    struct timeval tv;
    fd_set set;
    int joy_fd;

    // Big while loop opens, publishes
    while(1)
    {
        bool start_flag = true;
        bool publish_now = false;
        while (true)
        {
            joy_fd = open(JS_DEV, O_RDONLY);
            if (joy_fd != -1)
            {
                close(joy_fd);
                joy_fd = open(JS_DEV, O_RDONLY);
            }
            if (joy_fd != -1)
                break;
            sleep(1.0);
        }

        tv.tv_sec = 1;
        tv.tv_usec = 0;
        double val; //Temporary variable to hold event values
        while (1)
        {
            FD_ZERO(&set);
            FD_SET(joy_fd, &set);

            //ROS_INFO("Select...");
            int select_out = select(joy_fd+1, &set, NULL, NULL, &tv);
            //ROS_INFO("Tick...");
            if (select_out == -1)
            {
                tv.tv_sec = 0;
                tv.tv_usec = 0;
                //ROS_INFO("Select returned negative. %i", ros::isShuttingDown());
                continue;
                //break; // Joy is probably closed. Not sure if this case is useful.
            }

            if (FD_ISSET(joy_fd, &set))
            {
                if (read(joy_fd, &event, sizeof(js_event)) == -1 && errno != EAGAIN)
                    break; // Joy is probably closed. Definitely occurs.

                //ROS_INFO("Read data...");
                switch(event.type)
                {
                    case JS_EVENT_BUTTON:
                    case JS_EVENT_BUTTON | JS_EVENT_INIT:
                        joystick.buttons[event.number] = (event.value ? 1 : 0);
                        // For initial events, wait a bit before sending to try to catch
                        // all the initial events.
                        if(!(event.type & JS_EVENT_INIT) && event.number == 7) //第8个按钮按状态发生变化,开始发送手柄数据
                            start_flag = true;
                        //if(!(event.type & JS_EVENT_INIT) && event.number == 6) //第7个按钮按状态发生变化,停止发送手柄数据
                            //start_flag = false;
                        if (!(event.type & JS_EVENT_INIT))//有按键状态发生变化
                            publish_now = true;
                        break;
                    case JS_EVENT_AXIS:
                    case JS_EVENT_AXIS | JS_EVENT_INIT:
                        val = event.value;
                        if(val > unscaled_deadzone)
                            val -= unscaled_deadzone;
                        else if(val < -unscaled_deadzone)
                            val += unscaled_deadzone;
                        else
                            val=0;
                        joystick.axes[event.number]= val * scale;
                        if(!(event.type & JS_EVENT_INIT))//有按键状态发生变化
                            publish_now = true;
                        break;
                    default:
                        break;
                }
            }
            //发送手柄数据
            if (publish_now && start_flag) {
                qDebug() << joystick.buttons[0] << "," << joystick.buttons[1] << "," << joystick.buttons[2] << "," << joystick.buttons[3] <<
                            "," << joystick.buttons[4] << "," << joystick.buttons[5] << "," << joystick.buttons[6] << "," << joystick.buttons[7]
                          << "," << joystick.buttons[8]<< "," << joystick.buttons[9]<< "," << joystick.buttons[10]<< "," << joystick.buttons[11];

                qDebug() << joystick.axes[0] << "," << joystick.axes[1] << "," << joystick.axes[2] << "," << joystick.axes[3] << ","
                         << joystick.axes[4] << "," << joystick.axes[5] << "," << joystick.axes[6] << "," << joystick.axes[7];
                publish_now = false;
            }
        } // End of Joy open loop.
        close(joy_fd);
    }

}
