#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QDesktopWidget>
#include "windows.h"
#include <sapi.h>
#include <QDebug>

extern ISpVoice * pVoice;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),file("C:\\Users\\dell\\Desktop\\t\\t.txt"),bPause(false),bStop(false)
{
    setupUi(this);
    connect(m_btnStart,SIGNAL(clicked(bool)),SLOT(onStart()));
    connect(m_btnPause,SIGNAL(clicked(bool)),SLOT(onPause()));
    connect(m_btnAdd,SIGNAL(clicked(bool)),SLOT(OnAddRate()));
    connect(m_btnDec,SIGNAL(clicked(bool)),SLOT(OnDecRate()));
    connect(m_btnStop,SIGNAL(clicked(bool)),SLOT(OnStop()));
    connect(m_btnClose,SIGNAL(clicked(bool)),SLOT(OnClose()));
    connect(m_btnSpeed,SIGNAL(clicked(bool)),SLOT(OnSkip()));
    connect(&waitthread,SIGNAL(Speakdone()),this,SLOT(OnSpeakDone()),Qt::QueuedConnection);

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

    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    QRect rect = pDesktopWidget->screenGeometry();

    this->move(rect.width() - this->width(),0);





    ////////////////////////////////
    //COM初始化：
    if (FAILED(::CoInitialize(NULL)))
        return ;

    //获取ISpVoice接口：
    HRESULT hr  = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
    if( FAILED( hr ) )
        return;

    pVoice->SetRate(5);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
        return;
    }
}

MainWindow::~MainWindow()
{
#if 0
    myT.terminate();
#else
    waitthread.terminate();
#endif

    pVoice->Release();
   //千万不要忘记：
   ::CoUninitialize();
}

void MainWindow::TrayIconAction(QSystemTrayIcon::ActivationReason reason)
//参数是Qt的保留字，表示对托盘图标的操作，该槽函数功能只实现了单击功能，可以实现其他功能比如双击、中击，具体可查文档或者Qt自带demo(关键字tray)
{
    if (reason==QSystemTrayIcon::Trigger)
        this->showNormal();//如果对图标进行单击，则显示正常大小的窗口
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
void MainWindow::OnClose()
{
    this->hide();
}



////////////////////////////////////////////////

void MainWindow::OnAddRate()
{
#if 0
     myT.add();
#else
    LONG rate;
    pVoice->GetRate(&rate);
    pVoice->SetRate(rate+1);
#endif
}

void MainWindow::OnDecRate()
{
#if 0
    myT.dec();
#else
    LONG rate;
    pVoice->GetRate(&rate);
    pVoice->SetRate(rate-1);
#endif
}

//////////////////////////////////////////



void MainWindow::onStart()
{
    file.seek(0);
#if 0
    myT.setText(m_text->text());
#else
    OnStop();
    dospeak(QString("第%1章").arg(m_text->text()));

#endif
}

void MainWindow::onPause()
{
#if 0
    myT.OnPause();
#else
    if(bStop)
        return;
//    struct SPVOICESTATUS Status;
//    pVoice->GetStatus(&Status,NULL);
//    if(Status.dwRunningState)
    if(!bPause)
        pVoice->Pause();
    else
        pVoice->Resume();

    bPause = !bPause;
#endif
}



void MainWindow::OnStop()
{
#if 0
    myT.stop();
#else
    pVoice->Speak( NULL, SPF_PURGEBEFORESPEAK, 0);
    bStop = true;
#endif

}

void MainWindow::OnSkip()
{

#if 0
    myT.speed(3000);
#else
    if(bStop || bPause)
        return;

    WCHAR szGarbage[] = L"Sentence";
    pVoice->Skip(szGarbage,1,0);
#endif
}

void MainWindow::OnSpeakDone()
{
    if(bStop)
        return;
    qDebug()<<"one SpeakDone,next-->";
    dospeak();
}

void MainWindow::dospeak(QString beginText)
{
    static wchar_t array[1024]={0};
    while(!file.atEnd())
    {
        QByteArray line = file.readLine(1024);
        QString str(line);
        if(!beginText.isEmpty() && -1 == str.indexOf(beginText))
        {
            continue;
        }
        beginText.clear();

        qDebug()<<"line size:"<<line.size();

        memset(array,0,sizeof(array));
        str.toWCharArray(array);

        ULONG ret = 0;
        bStop = false;
        pVoice->Speak( array, SPF_ASYNC, &ret);
        qDebug()<<ret;

        if(ret % 40 == 0)
        {
            if(!waitthread.isRunning())
                waitthread.start();
            return;
        }
    }
}



//            DWORD dwThreadId;
//            CreateThread(NULL, 0, SpeakWaiting, pVoice, 0, &dwThreadId);
//DWORD WINAPI SpeakWaiting(_In_ LPVOID lpParameter)
//{
//    ISpVoice * pVoice = static_cast<ISpVoice*>(lpParameter);
//    pVoice->WaitUntilDone(INFINITE);

//    return 0;
//}
