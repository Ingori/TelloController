#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <memory>
#include <tellodriver.h>
#include <QInputEvent>
//#include <unordered_map>

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
};
#endif // MAINWINDOW_H
