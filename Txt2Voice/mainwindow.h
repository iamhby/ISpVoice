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

    void dospeak(QString beginText = "");
public slots:
    void onStart();
    void onPause();
    void OnAddRate();
    void OnDecRate();
    void OnStop();
    void OnClose();
    void OnSkip();

    void TrayIconAction(QSystemTrayIcon::ActivationReason reason);

    void OnSpeakDone();
private:
    //最小化到托盘----
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
private:
    VoiceThread myT;
    WaitThread  waitthread;
    QSystemTrayIcon *tray;
    QMenu* menu;
    QAction * reset;
    QAction*quit;

    QFile file;
    bool bPause;
    bool bStop;
};

#endif // MAINWINDOW_H
