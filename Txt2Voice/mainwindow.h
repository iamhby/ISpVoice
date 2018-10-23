#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "voicethread.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
class MainWindow : public QMainWindow ,public Ui_MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onStart(bool v);
    void onPause(bool v);
    void onAdd();
    void OnDec();
    void OnStop();
    void OnClose();
    void OnSpeed();

    void TrayIconAction(QSystemTrayIcon::ActivationReason reason);
private:
    //最小化到托盘----
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
private:
    VoiceThread myT;
    QSystemTrayIcon *tray;
    QMenu* menu;
    QAction * reset;
    QAction*quit;
};

#endif // MAINWINDOW_H
