#ifndef XSTRING_H
#define XSTRING_H


#include <QGraphicsEffect>
#include "xhighlight.h"
#include "xvibrator.h"

namespace vg
{
    class XString : public QObject, public QGraphicsRectItem
    {
        Q_OBJECT
    public:
        enum StringType
        {
            Golden = 0, // ?
            Steel,
            Nylon
        };

        StringType stringType = Steel;

        int thickness = 2;
        int highlightSize = 30;
        //Use a much faster (and uglier) strokePath instead of gradient fill
        bool _fastPaint = false;
        bool _vibrate = true;
        const int noteNameSize = 30;


        XString(QGraphicsItem* parent);
        XHighlight* highlight();

        void stopVibrating();
        void pluck(float position);
        void showHighlight(float position);
        void setNoteText(const QString& noteText);

    protected:
        XHighlight* _highlight = nullptr;
        XHighlight* _noteName = nullptr;


        void paintLineString(QPainter *painter);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QPainterPath generatePath();
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        void reposition();

        //Vibration
        float pluckPosition = 0.0f;
        Vibrator vibrator;
        void startVibrationTimer();
        void addBlurEffect();
        void fastPaint(QPainter *painter);
        void gradientPaint(QPainter *painter);
    private slots:
        void vibrationCallback();
    };
}
#endif // XSTRING_H
