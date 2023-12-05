//
// Created by AC on 2023/11/30.
//

#include "audioplaythread.h"
#include <QApplication>
#include <QThread>
#include <QDebug>

#ifdef __linux__
#include "utils/wavPlayer/play_wav.h"
#endif

void AudioPlayThread::playAudio() {
#ifndef __linux__
    startSound->play();
#else
    // play_wav_signal(":img/audio_thanks.wav");
    int ret = system("aplay /home/aichao/FRAM/audio_thanks.wav");
    if (ret < 0) {
        qDebug() << "播放语音失败";
    }
    QThread::msleep(1000);
    emit playAudioFinished();
#endif
}

AudioPlayThread::AudioPlayThread(QObject *parent) : QObject(parent) {
#ifndef __linux__
    startSound = new QSoundEffect(this);
    startSound->setSource(QUrl::fromLocalFile(":img/audio_thanks.wav"));
    startSound->setVolume(0.25f);
#endif
}
