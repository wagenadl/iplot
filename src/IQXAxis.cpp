// IQXAxis.cpp

#include "IQXAxis.h"
#include <QPainter>

IQXAxis::IQXAxis(QWidget *parent): IQBaseAxis(parent) {
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
}

QSize IQXAxis::sizeHint() const {
  if (title.isEmpty())
    return QSize(10, 20);
  else
    return QSize(10, 40);
  // could be smarter
}

void IQXAxis::paint(QPainter &p) {
  QRect r0 = contentsRect();
  int x0 = r0.left();
  int x1 = r0.right();
  double dd = tickStep();
  int y0 = r0.top();
  p.drawLine(r0.topLeft(), r0.topRight());
  for (double d=firstTick(); d<=max; d+=dd) {
    int x = int((d-min)/(max-min)*(x1-x0) + x0);
    p.drawLine(QPoint(x, y0), QPoint(x, y0+3));
    p.drawText(QRect(QPoint(x-100, y0+4), QPoint(x+100, y0+30)),
               Qt::AlignTop | Qt::AlignHCenter,
               QString::number(d));
  }
  if (!title.isEmpty())
    p.drawText(r0, Qt::AlignBottom | Qt::AlignHCenter, title);
}
