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

//    QUdpSocket* stream_sock = new QUdpSocket;
//    stream_sock->abort();
//    if(!stream_sock->bind(QHostAddress("0.0.0.0"), 11111, QUdpSocket::DontShareAddress))
//        throw std::invalid_argument("Can't bind to socket: 0.0.0.0:11111");
//    stream_sock->open(QIODevice::ReadOnly);
////    stream_sock->seek(qint64(0));

//    player = new QMediaPlayer;
    player = new QMediaPlayer(nullptr, QMediaPlayer::StreamPlayback);
//    connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
//        [this](QMediaPlayer::Error error){ PlayerError(error); });
//    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
//    connect(player, SIGNAL(networkConfigurationChanged(QMediaPlayer::MediaStatus)), this, SLOT(onNetworkConfigurationChanged(QMediaPlayer::MediaStatus)));
//    player->setMedia(QUrl::fromLocalFile("D:/Od/OneDrive/MyProjects/Tello/media/Serenity.mp4"));
//    player->setMedia(QMediaContent(), stream_sock);
    player->setMedia(QUrl::fromLocalFile("D:/Od/OneDrive/MyProjects/Tello/media/z_raw_video"));

//    player = new QMediaPlayer(nullptr, QMediaPlayer::StreamPlayback);
//    QFile file("D:/Od/OneDrive/MyProjects/Tello/media/Serenity.mp4");
//    file.open(QFile::ReadOnly);
//    player->setMedia(QMediaContent(), &file);

    videoWidget = new QVideoWidget;
    videoWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    ui->verticalLayout_7->addWidget(videoWidget);

    player->setVideoOutput(videoWidget);

    videoWidget->show();
    player->play();

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
        SetTelemetry(datagram);
    });
//    tello->SetStreamCB([this](const QNetworkDatagram& datagram)
//    {
//        static int x = 0;
//        if(x > 10000)
//        {
//            return;
//        }

//        setWindowTitle("TelloController " + QString::number(x++));

//        array += datagram.data();
//    });
}

MainWindow::~MainWindow()
{
    delete videoWidget;
    delete player;
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
                            "A:         Поворот против часовой стрелки.\r\n"
                            "D:         Поворот по часовой стрелке.\r\n\r\n"
                            "Shift + Up:    Переворот вперед.\r\n"
                            "Shift + Down:  Переворот назад.\r\n"
                            "Shift + Left:  Переворот влево.\r\n"
                            "Shift + Right: Переворот вправо.\r\n"
                            "Shift + W:     Получить параметры wifi.\r\n"
                            "Shift + S:     Получить значение скорости.\r\n"
                            "Shift + P:     Мгновенное отключение двигателей(Осторожно!).\r\n"
                             );
}

void MainWindow::SetTelemetry(const QNetworkDatagram &datagram)
{
    std::int32_t pitch, roll, yaw, vgx, vgy, vgz, templ, temph, tof, h, bat, time;
    double baro, agx, agy, agz;

    sscanf(datagram.data().data(),
           "pitch:%d;roll:%d;yaw:%d;vgx:%d;vgy:%d;vgz:%d;templ:%d;temph:%d;"
           "tof:%d;h:%d;bat:%d;baro:%lf;time:%d;agx:%lf;agy:%lf;agz:%lf;\r\n",
           &pitch, &roll, &yaw, &vgx, &vgy, &vgz, &templ, &temph,
           &tof, &h, &bat, &baro, &time, &agx, &agy, &agz);

    ui->pitch->setText("pitch: " + QString::number(pitch) + " ");
    ui->roll->setText("roll: " + QString::number(roll) + " ");
    ui->yaw->setText("yaw: " + QString::number(yaw) + " ");
    ui->vgx->setText("vgx: " + QString::number(vgx) + " см/с");
    ui->vgy->setText("vgy: " + QString::number(vgy) + " см/с");
    ui->vgz->setText("vgz: " + QString::number(vgz) + " см/с");

    if(templ >= 80)
        ui->templ->setStyleSheet("QLabel { color : red; }");
    else
        ui->templ->setStyleSheet("QLabel { color : black; }");
    ui->templ->setText("Temp lowest: " + QString::number(templ) + " °C");

    if(temph >= 80)
        ui->temph->setStyleSheet("QLabel { color : red; }");
    else
        ui->temph->setStyleSheet("QLabel { color : black; }");
    ui->temph->setText("Temp highest: " + QString::number(temph) + " °C");

    ui->tof->setText("TOF distance: " + QString::number(tof) + " см");
    ui->h->setText("Height: " + QString::number(h) + " см");

    if(bat <= 20)
        ui->bat->setStyleSheet("QLabel { color : red; }");
    else
        ui->bat->setStyleSheet("QLabel { color : black; }");
    ui->bat->setText("Battery: " + QString::number(bat) + " %");

    ui->time->setText("Time: " + QString::number(time) + " с");
    ui->baro->setText("Barometer: " + QString::number(baro) + " см");
    ui->agx->setText("agx: " + QString::number(agx) + " см²/с");
    ui->agy->setText("agy: " + QString::number(agy) + " см²/с");
    ui->agz->setText("agz: " + QString::number(agz) + " см²/с");
}

//void MainWindow::PlayerError(QMediaPlayer::Error error)
//{
//    qDebug() << "media error: " << error;
//}

//void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus media_status)
//{
//    qDebug() << "media status changed: " << media_status;
//}

