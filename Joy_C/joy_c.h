#ifndef JOY_C_H
#define JOY_C_H

#include <QThread>
#include <unistd.h>
#include <linux/joystick.h>
#include <fcntl.h>

#define JS_DEV   "/dev/input/js0"

typedef struct {
    unsigned char buttons[16];
    float  axes[8];
} joystick_t;

class Joy_C : public QThread
{
    Q_OBJECT

public:
    Joy_C();
    ~Joy_C();

    void run();

private:
    double deadzone_;
    joystick_t joystick;

};

#endif // JOY_C_H
