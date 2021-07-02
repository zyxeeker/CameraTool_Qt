#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "socket_test.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void refresh_list();
    void device_detail();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    modu::socket_test *m_socket;
    QMap<QString,QString> m_devices;
};
#endif // MAINWINDOW_H
