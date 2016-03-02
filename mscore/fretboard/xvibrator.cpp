#include "xvibrator.h"
#include <math.h>

namespace vg
{
    void Vibrator::startVibrating(float maxAmplitude)
    {
        amplitude = maxAmplitude;
        phase = M_PI / 2.0f;

        timer.stop();
        timer.start(1000/fps);
    }

    float Vibrator::tick()
    {
        updatePhase();
        updateAmplitude();
        return amplitude;
    }

    void Vibrator::updatePhase()
    {
        const float phaseStep = M_PI / 2;

        phase += phaseStep;

        if (phase > M_PI)
            phase -= 2.0f * M_PI;
    }

    void Vibrator::updateAmplitude()
    {
        // this determines the decay of the visible string vibration
        float decayPerFrame = fps * vibrationDurationMS / 1000.0;

        //Reduce amplitude by a small amount each frame until we reach almost zero
        amplitude -= amplitude/decayPerFrame;
    }

    void Vibrator::stop()
    {
        timer.stop();
        phase = 0.0;
        amplitude = 0.0;
    }

    float Vibrator::delta()
    {
        return sin (phase) * amplitude;
    }
}
