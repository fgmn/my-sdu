#include "media.h"

media::media()
{
    qDebug()<<"This is a media player";
}

//请选择旅客编号
void media::playSeq(){
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/seq.mp3").absoluteFilePath()));
    player->setVolume(100);
    player->play();
}
//请选择起点终点
void media::playChoose(){
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/choose.mp3").absoluteFilePath()));
    player->setVolume(100);
    player->play();
}
//无法查到合适路径
void media::playWarning(){
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/warning.mp3").absoluteFilePath()));
    player->setVolume(100);
    player->play();
}
//请先完成当前行程
void media::playFinish(){
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/finish.mp3").absoluteFilePath()));
    player->setVolume(100);
    player->play();
}
//请从当前位置处出发
void media::playWstart(){
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/wstart.mp3").absoluteFilePath()));
    player->setVolume(100);
    player->play();
}
//起点终点不可以相同
void media::playCritical(){
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/critical.mp3").absoluteFilePath()));
    player->setVolume(100);
    player->play();
}
//播报城市到站
void media::playCity(int seq){
    QMediaPlayer *player = new QMediaPlayer;
    switch(seq){
    case 0:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/shanghai.mp3").absoluteFilePath()));break;
    case 1:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/beijing.mp3").absoluteFilePath()));break;
    case 2:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/cangzhou.mp3").absoluteFilePath()));break;
    case 3:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/tianjin.mp3").absoluteFilePath()));break;
    case 4:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/lanzhou.mp3").absoluteFilePath()));break;
    case 5:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/wulumuqi.mp3").absoluteFilePath()));break;
    case 6:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/lasa.mp3").absoluteFilePath()));break;
    case 7:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/nanjing.mp3").absoluteFilePath()));break;
    case 8:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/changchun.mp3").absoluteFilePath()));break;
    case 9:player->setMedia(QUrl::fromLocalFile(QFileInfo("musicsrc/chengdu.mp3").absoluteFilePath()));break;
    default:break;
    }
    player->setVolume(100);
    player->play();
}
