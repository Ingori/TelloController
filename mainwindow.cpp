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
    if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
        switch(event->key())
        {
            case Qt::Key_Up:    tello->Flip_F(); return;
            case Qt::Key_Down:  tello->Flip_B(); return;
            case Qt::Key_Left:  tello->Flip_L(); return;
            case Qt::Key_Right: tello->Flip_R(); return;
            default: return;
        }

    static const int coef = 50;
    static const int angle = 30;
    switch(event->key())
    {
        case Qt::Key_F1:    Help(); return;
        case Qt::Key_I:     Init(); return;

        case Qt::Key_V:     VideoStream(); return;

        case Qt::Key_T:     tello->Takeoff(); return;
        case Qt::Key_L:     tello->Land(); return;
        case Qt::Key_P:     tello->Emergency(); return;

        case Qt::Key_Up:    tello->Forward(coef); return;
        case Qt::Key_Down:  tello->Back(coef); return;
        case Qt::Key_Left:  tello->Left(coef); return;
        case Qt::Key_Right: tello->Right(coef); return;
        case Qt::Key_W:     tello->Up(coef); return;
        case Qt::Key_S:     tello->Down(coef); return;
        case Qt::Key_Q:     tello->CCW(angle); return;
        case Qt::Key_E:     tello->CW(angle); return;
        default: return;
    }
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
                            "V:     Вкл./Выкл. видеострим."
                            "T:     Взлет.\r\n"
                            "L:     Посадка.\r\n"
                            "P:     Мгновенное отключение двигателей(Осторожно!).\r\n"
                            "Up:    Вперед.\r\n"
                            "Down:  Назад.\r\n"
                            "Left:  Влево.\r\n"
                            "Right: Вправо.\r\n"
                            "W:     Вверх.\r\n"
                            "S:     Вниз.\r\n"
                            "Q:     Поворот против часовой стрелки.\r\n"
                            "E:     Поворот по часовой стрелке."
                            );
}

