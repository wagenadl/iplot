// IQBaseAxis.h

#ifndef IQBASEAXIS_H

#define IQBASEAXIS_H

#include <QWidget>
#include <QPointF>

class IQBaseAxis: public QWidget {
public:
  IQBaseAxis(QWidget *parent=0);
  void setLimits(QPointF);
  void setTitle(QString);
protected:
  double tickStep() const;
  double firstTick() const;
protected:
  double min, max;
  QString title;
};

#endif
