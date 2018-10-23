#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    connect(m_btnStart,SIGNAL(clicked(bool)),SLOT(onStart(bool)));
    connect(m_btnPause,SIGNAL(clicked(bool)),SLOT(onPause(bool)));
    connect(m_btnAdd,SIGNAL(clicked(bool)),SLOT(onAdd()));
    connect(m_btnDec,SIGNAL(clicked(bool)),SLOT(OnDec()));
    connect(m_btnStop,SIGNAL(clicked(bool)),SLOT(OnStop()));
    connect(m_btnClose,SIGNAL(clicked(bool)),SLOT(OnClose()));
    connect(m_btnSpeed,SIGNAL(clicked(bool)),SLOT(OnSpeed()));

    this->setWindowTitle("test");

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint);


    tray=new QSystemTrayIcon(this);//初始化托盘对象tray
    tray->setIcon(QIcon(QPixmap("E:\\workerspace\\qt_pro\\Txt2Voice\\QQ0181018182239.png")));//设定托盘图标，引号内是自定义的png图片路径
    tray->show();//让托盘图标显示在系统托盘上
    QObject::connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(TrayIconAction(QSystemTrayIcon::ActivationReason)));//连接信号与槽，实现单击图标恢复窗口的功能，槽是自定义的槽


    //***初始化托盘菜单及功能***
    menu=new QMenu(this);//初始化菜单
    reset=new QAction(this);//初始化恢复窗口
    reset->setText("显示窗口");
    QObject::connect(reset,SIGNAL(triggered()),this,SLOT(showNormal()));//菜单中的显示窗口，单击连接显示窗口

    quit=new QAction(this);//初始化退出程序
    quit->setText("退出程序");
    QObject::connect(quit,SIGNAL(triggered()),qApp,SLOT(quit()));//菜单中的退出程序，单击连接退出</span>
    //qApp，是Qt自带的demo中的知识点，查了一下文档，qApp是Qt中所有app的指针，关闭它就可以关闭当前的程序</span>
    //之所以不用this，close()，是由于软件要求关闭改为最小化到托盘，所以close()的功能已经不再是关闭窗口的功能，所以要另寻方法
    //***将定义好的菜单加入托盘的菜单模块中***
    tray->setContextMenu(menu);
    menu->addAction(reset);
    menu->addAction(quit);


}



void MainWindow::TrayIconAction(QSystemTrayIcon::ActivationReason reason)
//参数是Qt的保留字，表示对托盘图标的操作，该槽函数功能只实现了单击功能，可以实现其他功能比如双击、中击，具体可查文档或者Qt自带demo(关键字tray)
{
    if (reason==QSystemTrayIcon::Trigger)
        this->showNormal();//如果对图标进行单击，则显示正常大小的窗口
}

MainWindow::~MainWindow()
{
    myT.terminate();
}

void MainWindow::onStart(bool v)
{
    myT.setText(m_text->text());
}

void MainWindow::onPause(bool v)
{
    myT.OnPause();
}

void MainWindow::onAdd()
{
    myT.add();
}

void MainWindow::OnDec()
{
    myT.dec();
}

void MainWindow::OnStop()
{
    myT.stop();
}

void MainWindow::OnClose()
{
    this->hide();
}

void MainWindow::OnSpeed()
{
    myT.speed(3000);
}

void MainWindow::changeEvent(QEvent *e)
{
    if((e->type()==QEvent::WindowStateChange)&&this->isMinimized())
    {
        this->hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}
