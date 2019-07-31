#ifndef JOY2_H
#define JOY2_H

#include <unistd.h>
#include <math.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <diagnostic_updater/diagnostic_updater.h>
#include "ros/ros.h"
#include <sensor_msgs/Joy.h>
#include <sensor_msgs/JoyFeedbackArray.h>
#include <linux/input.h>
#include <unistd.h>
#include<QThread>
#include <fstream>

#define DEVICE_KEY1 "Joystick"  //摇动柄
#define DEVICE_KEY2 "G29"  //
#define EVENT_KEY   "Handlers"
#define DEVICE_PATH "/proc/bus/input/devices"

class Joy : public QThread
{
public:
    Joy();
    ~Joy();

    void set_feedback(const sensor_msgs::JoyFeedbackArray::ConstPtr& msg);
    void fun();

private:
    ros::NodeHandle nh_;
    bool open_;
    bool sticky_buttons_;
    bool default_trig_val_;
    std::string joy_dev_;
    std::string joy_dev_2;
    std::string joy_dev_name_;
//  std::string joy_def_ff_;
    double deadzone_;
    double autorepeat_rate_;   // in Hz.  0 for no repeat.
    double coalesce_interval_; // Defaults to 100 Hz rate limit.
    int event_count_;
    int pub_count_;
    ros::Publisher pub_;
    double lastDiagTime_;

//  int ff_fd_;
//  struct ff_effect joy_effect_;
    bool update_feedback_;

//  diagnostic_updater::Updater diagnostic_;
    void diagnostics(diagnostic_updater::DiagnosticStatusWrapper& stat);
    std::string get_dev_by_joy_name(const std::string& joy_name);
    void send_msg();
    void findDevice();

};

#endif // JOY_H
