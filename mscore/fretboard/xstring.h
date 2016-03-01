#ifndef XSTRING_H
#define XSTRING_H


#include <QGraphicsEffect>
#include "xhighlight.h"

namespace vg
{


    class XString : public QObject, public QGraphicsRectItem
    {
        Q_OBJECT
    public:

        enum StringType
        {
            Golden = 0, //Brass ?
            Steel,
            Nylon
        };

        StringType stringType = Steel;


        int thickness = 2;
        int dotSize = 30;
        const int fps = 60;
        //Use a much faster (and uglier) strokePath instead of gradient fill
        bool _fastPaint = false;

        XString(QGraphicsItem* parent);
        XHighlight* highlight();

        void stopVibrating();
        void pluck(float position);

    protected:
        XHighlight* _highlight = nullptr;

        void paintLineString(QPainter *painter);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QPainterPath generatePath();
        void mousePressEvent(QGraphicsSceneMouseEvent* event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

        //Vibration
        void updateAmplitude();
        void updatePhase();
        float amplitude = 0.0f;
        const float maxAmplitude = 10.0f;
        float phase = 0.0f;
        float pluckPosition = 0.0f;
        int vibrationDurationMS = 1000;
        QTimer* vibrationTimer = nullptr;
        void startVibrationTimer();
        void addBlurEffect();
        void fastPaint(QPainter *painter);
        void gradientPaint(QPainter *painter);
    private slots:
        void timerCallback();
    };
}
#endif // XSTRING_H
