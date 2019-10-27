#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <exception>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("TelloController");

    tello = std::make_unique<TelloDriver>();
    tello->SetCmdOutCB([this](const std::string& data)
    {
        ui->textBrowserCmd->append(QString::fromStdString(data));
    });
    tello->SetCmdInCB([this](const QNetworkDatagram& data)
    {
        ui->textBrowserCmd->append(data.data());
    });

    tello->SetTelemetryCB([this](const QNetworkDatagram& datagram)
    {
        std::int32_t pitch, roll, yaw, vgx, vgy, vgz, templ, temph, tof, h, bat, time;
        double baro, agx, agy, agz;

        sscanf(datagram.data().data(),
               "pitch:%d;roll:%d;yaw:%d;vgx:%d;vgy:%d;vgz:%d;templ:%d;temph:%d;"
               "tof:%d;h:%d;bat:%d;baro:%lf;time:%d;agx:%lf;agy:%lf;agz:%lf;\r\n",
               &pitch, &roll, &yaw, &vgx, &vgy, &vgz, &templ, &temph,
               &tof, &h, &bat, &baro, &time, &agx, &agy, &agz);

//        QString str_debug(datagram.data().data());
//        qDebug() << str_debug;

        ui->pitch->setText("pitch: " + QString::number(pitch) + " ");
        ui->roll->setText("roll: " + QString::number(roll) + " ");
        ui->yaw->setText("yaw: " + QString::number(yaw) + " ");
        ui->vgx->setText("vgx: " + QString::number(vgx) + " см/с");
        ui->vgy->setText("vgy: " + QString::number(vgy) + " см/с");
        ui->vgz->setText("vgz: " + QString::number(vgz) + " см/с");
        ui->templ->setText("Temp lowest: " + QString::number(templ) + " °C");
        ui->temph->setText("Temp highest: " + QString::number(temph) + " °C");
        ui->tof->setText("TOF distance: " + QString::number(tof) + " см");
        ui->h->setText("Height: " + QString::number(h) + " см");
        ui->bat->setText("Battery: " + QString::number(bat) + " %");
        ui->time->setText("Time: " + QString::number(time) + " с");
        ui->baro->setText("Barometer: " + QString::number(baro) + " см");
        ui->agx->setText("agx: " + QString::number(agx) + " см²/с");
        ui->agy->setText("agy: " + QString::number(agy) + " см²/с");
        ui->agz->setText("agz: " + QString::number(agz) + " см²/с");
    });

//    tello->SetStreamCB([this](const QNetworkDatagram& data)
//    {
////        static int x = 0;
////        setWindowTitle("TelloController " + QString::number(x++));
//    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->isAutoRepeat())
        return;

    if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
        switch(event->key())
        {
            case Qt::Key_Up:    tello->Flip_F(); return;
            case Qt::Key_Down:  tello->Flip_B(); return;
            case Qt::Key_Left:  tello->Flip_L(); return;
            case Qt::Key_Right: tello->Flip_R(); return;

            case Qt::Key_P:     tello->Emergency(); return;

            case Qt::Key_W:     tello->GetWIFI(); return;
            case Qt::Key_S:     tello->GetSpeed(); return;
            default: return;
        }

    switch(event->key())
    {
        case Qt::Key_F1:    Help(); return;
        case Qt::Key_I:     Init(); return;

        case Qt::Key_V:     VideoStream(); return;

        case Qt::Key_T:     tello->Takeoff(); return;
        case Qt::Key_L:     tello->Land(); return;

//        case Qt::Key_H:     tello->SetSpeed(100); return;

        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_W:
        case Qt::Key_S:
        case Qt::Key_A:
        case Qt::Key_D:
            keyProcess(event->key(), true); return;

        default: return;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return;

    switch(event->key())
    {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_W:
        case Qt::Key_S:
        case Qt::Key_A:
        case Qt::Key_D:
            keyProcess(event->key(), false); return;

        default: return;
    }
}

void MainWindow::keyProcess(int key, bool pressed)
{
    static bool Up = false;
    static bool Down = false;
    static bool Left = false;
    static bool Right = false;
    static bool W = false;
    static bool S = false;
    static bool A = false;
    static bool D = false;

    switch(key)
    {
        case Qt::Key_Up: Up = pressed; break;
        case Qt::Key_Down: Down = pressed; break;
        case Qt::Key_Left: Left = pressed; break;
        case Qt::Key_Right: Right = pressed; break;
        case Qt::Key_W: W = pressed; break;
        case Qt::Key_S: S = pressed; break;
        case Qt::Key_A: A = pressed; break;
        case Qt::Key_D: D = pressed; break;
        default: return;
    }

    static const int coef = 100;
    int a, b, c, d;
    a = b = c = d = 0;
    if(Right)
        a = coef;
    if(Left)
        a -= coef;
    if(Up)
        b = coef;
    if(Down)
        b -= coef;
    if(W)
        c = coef;
    if(S)
        c -= coef;
    if(D)
        d = coef;
    if(A)
        d -= coef;

    static int old_a = 0;
    static int old_b = 0;
    static int old_c = 0;
    static int old_d = 0;
    if(a == old_a && b == old_b && c == old_c && d == old_d)
        return;

    tello->SetRC(a, b, c, d);

    old_a = a;
    old_b = b;
    old_c = c;
    old_d = d;
}

void MainWindow::Init()
{
    try
    {
        tello->Init();
    }
    catch(const std::exception& ex)
    {
        QMessageBox::critical(nullptr, QString(), ex.what());
        std::exit(1);
    }
}

void MainWindow::VideoStream()
{
    static bool stream = true;
    if(stream)
        tello->Streamon();
    else
        tello->Streamoff();
    stream = !stream;
}

void MainWindow::Help()
{
    QMessageBox::information(nullptr, "Help",
                            "Нажмите \"I\" для инициализации Tello.\r\n"
                            "V:         Вкл./Выкл. видеострим.\r\n"
                            "T:         Взлет.\r\n"
                            "L:         Посадка.\r\n"
                            "Up:        Вперед.\r\n"
                            "Down:      Назад.\r\n"
                            "Left:      Влево.\r\n"
                            "Right:     Вправо.\r\n"
                            "W:         Вверх.\r\n"
                            "S:         Вниз.\r\n"
                            "Q:         Поворот против часовой стрелки.\r\n"
                            "E:         Поворот по часовой стрелке.\r\n\r\n"
                            "Shift + Up:    Переворот вперед.\r\n"
                            "Shift + Down:  Переворот назад.\r\n"
                            "Shift + Left:  Переворот влево.\r\n"
                            "Shift + Right: Переворот вправо.\r\n"
                            "Shift + W:     Получить параметры wifi.\r\n"
                            "Shift + S:     Получить значение скорости.\r\n"
                            "Shift + P:     Мгновенное отключение двигателей(Осторожно!).\r\n"
                            );
}

