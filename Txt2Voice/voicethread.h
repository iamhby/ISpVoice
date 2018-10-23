#ifndef VOICETHREAD_H
#define VOICETHREAD_H

#include <QObject>
#include <QFile>
#include<QThread>
class VoiceThread : public QThread
{
public:
    VoiceThread();
    ~VoiceThread();
    void setText(QString str);
    void OnPause();

    void add();
    void dec();
    void stop();
    void speed(int size);
protected:
    void run();

    QFile file;
    QString text;
    bool jump;
    bool pause;
    bool bstop;
    int rate;
    int speedsize;
};

#endif // VOICETHREAD_H
