//
// Created by AC on 2023/11/30.
//

#include "AudioPlayThread.h"
#include <QApplication>
#include <QThread>

#ifdef __linux__
#include "utils/wavPlayer/play_wav.h"
#endif

void AudioPlayThread::playAudio() {
#ifndef __linux__
    startSound->play();
#else
    // play_wav_signal(":img/audio_thanks.wav");
    system("aplay /home/aichao/FRAM/audio_thanks.wav");
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
