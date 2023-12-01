//
// Created by AC on 2023/11/30.
//

#pragma once

#include <QObject>
#include <QSoundEffect>


class AudioPlayThread : public QObject {
Q_OBJECT

public:
    explicit AudioPlayThread(QObject *parent = nullptr);

    ~AudioPlayThread() override = default;

signals:
    void playAudioFinished();

public slots:

    void playAudio();

private:
    QSoundEffect *startSound;
};