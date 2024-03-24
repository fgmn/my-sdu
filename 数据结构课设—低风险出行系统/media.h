#ifndef MEDIA_H
#define MEDIA_H
#include <QMediaPlayer>
#include <QFileInfo>
class media
{
public:
    media();
    static void playChoose();
    static void playSeq();
    static void playWarning();
    static void playFinish();
    static void playWstart();
    static void playCritical();
    static void playCity(int);
};

#endif // MEDIA_H
