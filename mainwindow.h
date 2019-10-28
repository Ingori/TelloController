#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <memory>
#include <tellodriver.h>
#include <QInputEvent>

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QIODevice>
#include <QBuffer>

class CustomFile : public QFile
{
public:
    CustomFile(const QString& file) : QFile(file) {}
    bool isSequential() const override;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void keyProcess(int key, bool pressed);
    void Init();
    void VideoStream();
    void Help();
    void SetTelemetry(const QNetworkDatagram& datagram);
    void PlayerError(QMediaPlayer::Error error);

private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus media_status);

private:
    Ui::MainWindow* ui;

    std::unique_ptr<TelloDriver> tello;


    QMediaPlayer* player;
    QVideoWidget* videoWidget;

//    QUdpSocket* stream_sock;
//    QByteArray array;
//    QBuffer* buffer;
};
#endif // MAINWINDOW_H
