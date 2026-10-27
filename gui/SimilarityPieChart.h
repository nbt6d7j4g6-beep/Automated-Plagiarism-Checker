
#ifndef SIMILARITYPIECHART_H
#define SIMILARITYPIECHART_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QStringList>
#include <vector>

using namespace std;

class SimilarityPieChart : public QWidget
{
public:
    explicit SimilarityPieChart(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setMinimumSize(350, 180);
        setMaximumHeight(220);
    }

    void setCounts(int high, int moderate, int low)
    {
        counts = {high, moderate, low};
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int total = counts[0] + counts[1] + counts[2];

        painter.drawText(
            10, 25,
            "Similarity Category Distribution"
        );

        if (total == 0)
        {
            painter.drawText(
                rect(),
                Qt::AlignCenter,
                "Run a comparison to display the chart"
            );
            return;
        }

        int diameter = qMin(width() - 180, height() - 50);
        diameter = qMax(100, diameter);

        QRect pieRect(
            20,
            (height() - diameter) / 2,
            diameter,
            diameter
        );

        QColor colors[] = {
            QColor("#dc2626"),
            QColor("#d97706"),
            QColor("#16a34a")
        };

        QStringList labels = {
            "High",
            "Moderate",
            "Low"
        };

        int startAngle = 90 * 16;

        for (int i = 0; i < 3; i++)
        {
            int spanAngle =
                (counts[i] * 360 * 16) / total;

            painter.setBrush(colors[i]);
            painter.setPen(Qt::white);

            painter.drawPie(
                pieRect,
                startAngle,
                -spanAngle
            );

            startAngle -= spanAngle;

            int legendY = 80 + i * 45;

            painter.setBrush(colors[i]);
            painter.drawRect(
                diameter + 40,
                legendY - 12,
                16,
                16
            );

            painter.setPen(Qt::black);

            double percentage =
                (counts[i] * 100.0) / total;

            painter.drawText(
                diameter + 65,
                legendY,
                QString("%1: %2 (%3%)")
                    .arg(labels[i])
                    .arg(counts[i])
                    .arg(percentage, 0, 'f', 1)
            );
        }
    }

private:
    vector<int> counts = {0, 0, 0};
};

#endif
