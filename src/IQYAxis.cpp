// IQYaxis.cpp

#include "IQYAxis.h"
#include <QPainter>

IQYAxis::IQYAxis(QWidget *parent): IQBaseAxis(parent) {
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
}

QSize IQYAxis::sizeHint() const {
  if (title.isEmpty())
    return QSize(40, 10);
  else
    return QSize(55, 10);
  // could be smarter
}

void IQYAxis::paint(QPainter &p) {
  QRect r0 = contentsRect();
  int y0 = r0.top();
  int y1 = r0.bottom();
  double dd = tickStep();
  int x0 = r0.right();
  p.drawLine(r0.topRight(), r0.bottomRight());
  for (double d=firstTick(); d<=max; d+=dd) {
    int y = int(y1 - (d-min)/(max-min)*(y1-y0));
    p.drawLine(QPoint(x0, y), QPoint(x0-3, y));
    p.drawText(QRect(QPoint(x0-100, y+100), QPoint(x0-5, y-100)),
               Qt::AlignRight | Qt::AlignVCenter,
               QString::number(d));
  }
  if (!title.isEmpty()) {
    p.rotate(-90);
    p.drawText(QRect(QPoint(-r0.bottom(), r0.left()),
                     QPoint(-r0.top(), r0.right())),
               Qt::AlignTop | Qt::AlignHCenter, title);
  }
}
