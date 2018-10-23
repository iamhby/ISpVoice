#include "voicethread.h"
#include "windows.h"
#include <sapi.h>
#include <QFile>
#include <QDebug>

ISpVoice * pVoice = NULL;

VoiceThread::VoiceThread():file("C:\\Users\\dell\\Desktop\\t\\t.txt")
{
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }

    pause = false;
    jump = false;
    rate = 4;
    bstop = false;
}

VoiceThread::~VoiceThread()
{

}

QString getN(int num)
{
    switch (num) {
    case 0: return "零";
    case 1: return "一";
    case 2: return "二";
    case 3: return "三";
    case 4: return "四";
    case 5: return "五";
    case 6: return "六";
    case 7: return "七";
    case 8: return "八";
    default:return "九";
    }

}

void VoiceThread::setText(QString str)
{
    bstop = false;
    int num = str.toInt();
    if(num <=1040)
    {
        text = "第";
        while(num>0)
        {
            if(num >= 1000)
            {
                text+=getN(num/1000)+"千";
                num = num %1000;
                if(num<100 && num>0)
                    text += "零";
            }
            else  if(num >= 100)
            {
                text+=getN(num/100)+"百";
                num = num %100;
                if(num<10&& num>0)
                    text += "零";
            }
            else  if(num >= 10)
            {
                text+=getN(num/10)+"十";
                num = num %10;
            }
            else
            {
                text+=getN(num%10);
                num = 0;
            }
        }

        text += "章";
    }
    else
    {
        text = QString("第%1章").arg(str);
    }
    qDebug()<<text;
//    return;
    jump = false;
    if(this->isRunning())
    {
        jump = true;
        pause = false;
    }
    else
    {
        this->start();
        file.seek(0);
    }
}

void VoiceThread::OnPause()
{
    pause = !pause;
}

void VoiceThread::add()
{
    rate++;
}

void VoiceThread::dec()
{
    rate--;
}

void VoiceThread::stop()
{
    bstop = true;
}

void VoiceThread::speed(int size)
{
    speedsize += size;
}

void VoiceThread::run()
{
    //COM初始化：
    if (FAILED(::CoInitialize(NULL)))
        return ;

    //获取ISpVoice接口：
    HRESULT hr  = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
    if( FAILED( hr ) )
        return;

    file.seek(0);
    static wchar_t array[4986]={0};
    bool bfind = false;
    while(!file.atEnd())
    {
        if(bstop)
            break;

        if(pause)
        {
            sleep(1);
            continue;
        }

        if(jump)
        {
            bfind = false;
            file.seek(0);
            jump = false;
        }

        QByteArray line = file.readLine(4986);
        QString str(line);
        if(!bfind &&  -1 == str.indexOf(text))
        {
            continue;
        }
        qDebug()<<"line size:"<<line.size();
        bfind = true;

        if(speedsize>0)
        {
            speedsize -= line.size();
            continue;
        }

        memset(array,0,sizeof(array));
        str.toWCharArray(array);
        pVoice->SetRate(rate);
        pVoice->Speak( array/*L"玩呗德阳麻将玩法规则-2018.08.15"*/, 0, NULL);
    }
    qDebug()<<"END";

    pVoice->Release();
   //千万不要忘记：
   ::CoUninitialize();
}
