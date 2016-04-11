#ifndef XVIBRATOR_H
#define XVIBRATOR_H


#include <QTimer>

namespace vg
{
    //Computes amplitude of vibration over time
    //
    class Vibrator : public QObject
    {
    public:
        void startVibrating(float maxAmplitude);
        float tick();
        void updatePhase();
        void updateAmplitude();
        void stop();

        float delta();

        float phase = 0.0;
        float amplitude = 0.0f;
        const int fps = 60;
        int vibrationDurationMS = 1000;
        QTimer timer;
    };
}

#endif // XVIBRATOR_H
