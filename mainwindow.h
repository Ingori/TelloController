#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <memory>
#include <tellodriver.h>
#include <QInputEvent>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QAbstractVideoSurface>
//#include <unordered_map>

class MyVideoSurface : public QAbstractVideoSurface
{
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const
    {
        Q_UNUSED(handleType);

        // Return the formats you will support
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB565;
    }

    bool present(const QVideoFrame &frame)
    {
        Q_UNUSED(frame);
        // Handle the frame and do your processing

        return true;
    }
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

private:
    Ui::MainWindow* ui;

    std::unique_ptr<TelloDriver> tello;


    QMediaPlayer* player;
    QVideoWidget* videoWidget;
    MyVideoSurface* surface;
};
#endif // MAINWINDOW_H
