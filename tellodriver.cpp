#include "tellodriver.h"

TelloDriver::TelloDriver()
{
    imp = std::make_unique<TelloDriverImp>();
}

void TelloDriver::Init()
{
    static const std::string cmd("command");
    imp->Init();
    imp->Execute(cmd);
}

void TelloDriver::Takeoff()
{
    static const std::string cmd("takeoff");
    imp->Execute(cmd);
}

void TelloDriver::Land()
{
    static const std::string cmd("land");
    imp->Execute(cmd);
}

void TelloDriver::Streamon()
{
    static const std::string cmd("streamon");
    imp->Execute(cmd);
}

void TelloDriver::Streamoff()
{
    static const std::string cmd("streamoff");
    imp->Execute(cmd);
}

void TelloDriver::Emergency()
{
    static const std::string cmd("emergency");
    imp->Execute(cmd);
}

bool TelloDriver::Up(int32_t x)
{
    if(x < 20 || x > 500)
        return false;

    static const std::string cmd("up");
    imp->Execute(MakeCmd() << cmd << x);
    return true;
}

bool TelloDriver::Down(int32_t x)
{
    if(x < 20 || x > 500)
        return false;

    static const std::string cmd("down");
    imp->Execute(MakeCmd() << cmd << x);
    return true;
}

bool TelloDriver::Left(int32_t x)
{
    if(x < 20 || x > 500)
        return false;

    static const std::string cmd("left");
    imp->Execute(MakeCmd() << cmd << x);
    return true;
}

bool TelloDriver::Right(int32_t x)
{
    if(x < 20 || x > 500)
        return false;

    static const std::string cmd("right");
    imp->Execute(MakeCmd() << cmd << x);
    return true;
}

bool TelloDriver::Forward(int32_t x)
{
    if(x < 20 || x > 500)
        return false;

    static const std::string cmd("forward");
    imp->Execute(MakeCmd() << cmd << x);
    return true;
}

bool TelloDriver::Back(int32_t x)
{
    if(x < 20 || x > 500)
        return false;

    static const std::string cmd("back");
    imp->Execute(MakeCmd() << cmd << x);
    return true;
}

bool TelloDriver::CW(int32_t x)
{
    if(x < 1 || x > 3600)
        return false;

    static const std::string cmd("cw");
    imp->Execute(MakeCmd() << cmd << x);
    return true;
}

bool TelloDriver::CCW(int32_t x)
{
    if(x < 1 || x > 3600)
        return false;

    static const std::string cmd("ccw");
    imp->Execute(MakeCmd() << cmd << x);
    return true;
}

void TelloDriver::Flip_L()
{
    static const std::string cmd("flip");
    static const std::string arg("l");
    imp->Execute(MakeCmd() << cmd << arg);
}

void TelloDriver::Flip_R()
{
    static const std::string cmd("flip");
    static const std::string arg("r");
    imp->Execute(MakeCmd() << cmd << arg);
}

void TelloDriver::Flip_F()
{
    static const std::string cmd("flip");
    static const std::string arg("f");
    imp->Execute(MakeCmd() << cmd << arg);
}

void TelloDriver::Flip_B()
{
    static const std::string cmd("flip");
    static const std::string arg("b");
    imp->Execute(MakeCmd() << cmd << arg);
}

bool TelloDriver::Go(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t speed)
{
    if((x < 20 || x > 500) || (y < 20 || y > 500) || (z < 20 || z > 500))
        return false;

    if(speed < 10 || speed > 100)
        return false;

    static const std::string cmd("go");
    imp->Execute(MakeCmd() << cmd << x << y << z << speed);
    return true;
}

bool TelloDriver::Curve(std::int32_t x1, std::int32_t y1, std::int32_t z1,
                        std::int32_t x2, std::int32_t y2, std::int32_t z2,
                        std::int32_t speed)
{
    if((x1 < 20 || x1 > 500) || (y1 < 20 || y1 > 500) || (z1 < 20 || z1 > 500))
        return false;

    if((x2 < 20 || x2 > 500) || (y2 < 20 || y2 > 500) || (z2 < 20 || z2 > 500))
        return false;

    if(speed < 10 || speed > 60)
        return false;

    static const std::string cmd("curve");
    imp->Execute(MakeCmd() << cmd << x1 << y1 << z1 << x2 << y2 << z2 << speed);
    return true;
}

bool TelloDriver::SetSpeed(std::int32_t speed)
{
    if(speed < 10 || speed > 100)
        return false;

    static const std::string cmd("speed");
    imp->Execute(MakeCmd() << cmd << speed);
    return true;
}

bool TelloDriver::SetRC(std::int32_t a, std::int32_t b, std::int32_t c, std::int32_t d)
{
    if((a < -100 || a > 100) || (b < -100 || b > 100) || (c < -100 || c > 100) || (d < -100 || d > 100))
        return false;

    static const std::string cmd("rc");
    imp->Execute(MakeCmd() << cmd << a << b << c << d);
    return true;
}

bool TelloDriver::SetWIFI(const std::string& ssid, const std::string& pass)
{
    static const std::string cmd("wifi");
    imp->Execute(MakeCmd() << cmd << ssid << pass);
    return true;
}

void TelloDriver::GetSpeed()
{
    static const std::string cmd("speed?");
    imp->Execute(cmd);
}

void TelloDriver::GetBattery()
{
    static const std::string cmd("battery?");
    imp->Execute(cmd);
}

void TelloDriver::GetTime()
{
    static const std::string cmd("time?");
    imp->Execute(cmd);
}

void TelloDriver::GetHeight()
{
    static const std::string cmd("height?");
    imp->Execute(cmd);
}

void TelloDriver::GetTemp()
{
    static const std::string cmd("temp?");
    imp->Execute(cmd);
}

void TelloDriver::GetAttitude()
{
    static const std::string cmd("attitude?");
    imp->Execute(cmd);
}

void TelloDriver::GetBaro()
{
    static const std::string cmd("baro?");
    imp->Execute(cmd);
}

void TelloDriver::GetAcceleration()
{
    static const std::string cmd("acceleration?");
    imp->Execute(cmd);
}

void TelloDriver::GetTof()
{
    static const std::string cmd("tof?");
    imp->Execute(cmd);
}

void TelloDriver::GetWIFI()
{
    static const std::string cmd("wifi?");
    imp->Execute(cmd);
}
