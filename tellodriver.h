#ifndef TELLODRIVER_H
#define TELLODRIVER_H

#include <memory>
#include <string>
#include <sstream>
#include <tellodriverimp.h>

class TelloDriver
{
public:
    TelloDriver();

    // Сначала устанавливаем колбэки если необходимо
    void SetCmdOutCB(CbTelloOutput cb) { imp->SetCmdOutCB(cb); }
    void SetCmdInCB(CbTelloInput cb) { imp->SetCmdInCB(cb); }
    void SetTelemetryCB(CbTelloInput cb) { imp->SetTelemetryCB(cb); }
    void SetStreamCB(CbTelloInput cb) { imp->SetStreamCB(cb); }

    // Затем вызываем Init()
    void Init();

    void Takeoff();   // Взлет
    void Land();      // Посадка
    void Streamon();  // Вкл. видео
    void Streamoff(); // Выкл. видео
    void Emergency(); // Мгновенная остановка двигателей

    // Перемещения [20-500]см
    bool Up(std::int32_t x);
    bool Down(std::int32_t x);
    bool Left(std::int32_t x);
    bool Right(std::int32_t x);
    bool Forward(std::int32_t x);
    bool Back(std::int32_t x);

    bool CW(std::int32_t x);  // Поворот по часовой [1-3600]
    bool CCW(std::int32_t x); // Поворот против часовой [1-3600]

    // Перевороты: влево, вправо, вперед, назад
    void Flip_L();
    void Flip_R();
    void Flip_F();
    void Flip_B();

    // Двигатья до (x,y,z) со скоростью speed [10-100]см/с
    bool Go(std::int32_t x, std::int32_t y, std::int32_t z, std::int32_t speed);

    // Tello fly a curve defined by the current and two given coordinates with speed (cm/s).
    // If the arc radius is not within the range of 0.5-10 meters, it responses false
    // x1, x2: 20-500
    // y1, y2: 20-500
    // z1, z2: 20-500
    // speed: 10-60
    // x/y/z can’t be between -20 – 20 at the same time.
    bool Curve(std::int32_t x1, std::int32_t y1, std::int32_t z1,
                      std::int32_t x2, std::int32_t y2, std::int32_t z2,
                      std::int32_t speed );

    bool SetSpeed(std::int32_t speed); // Задать скорость [10-100]см/c

    // RC a b c d
    // Send RC control via four channels.
    // a: left/right (-100~100)
    // b: forward/backward (-100~100)
    // c: up/down (-100~100)
    // d: yaw (-100~100)
    bool SetRC(std::int32_t a, std::int32_t b, std::int32_t c, std::int32_t d);

    bool SetWIFI(const std::string& ssid, const std::string& pass);

    void GetSpeed();        // Get current speed (cm/s). X: 1-100
    void GetBattery();      // Get current battery percentage. X: 0-100
    void GetTime();         // Get current fly time (s)
    void GetHeight();       // Get height (cm). X: 0-3000
    void GetTemp();         // Get temperature (℃). X: 0-90
    void GetAttitude();     // Get IMU attitude data. Pitch roll yaw
    void GetBaro();         // Get barometer value (m)
    void GetAcceleration(); // Get IMU angular acceleration data (0.001g). X Y Z
    void GetTof();          // Get distance value from TOF（cm）. X: 30-1000
    void GetWIFI();         // Get Wi-Fi SNR

private:
    std::unique_ptr<TelloDriverImp> imp;
};

class MakeCmd
{
public:
    template<class T>
    MakeCmd& operator<< (const T& arg)
    {
        stream << arg << " ";
        return *this;
    }

    operator std::string() const
    {
        return stream.str();
    }

private:
    std::stringstream stream;
};

#endif // TELLODRIVER_H
