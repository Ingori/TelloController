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
    void Init();
    void VideoStream();
    void Help();

private:
    Ui::MainWindow* ui;

    std::unique_ptr<TelloDriver> tello;
//    std::unordered_map<Qt::Key, std::func<void(
};
#endif // MAINWINDOW_H
