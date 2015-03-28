// IQBaseAxis.cpp

#include "IQBaseAxis.h"
#include <math.h>

IQBaseAxis::IQBaseAxis(QWidget *parent): QWidget(parent) {
  min = 0;
  max = 1;
}

void IQBaseAxis::setLimits(QPointF m) {
  min = m.x();
  max = m.y();
}

double IQBaseAxis::tickStep() const {
  double t0 = (max-min) / 7;
  double fac = pow(10, floor(log10(t0)));
  double mul = t0/fac;
  if (mul>=5)
    mul = 5;
  else if (mul>=2.5)
    mul = 2.5;
  else if (mul>=2)
    mul = 2;
  else
    mul = 1;
  return fac*mul;
}

double IQBaseAxis::firstTick() const {
  double ts = tickStep();
  return ts*ceil(min/ts);
}

void IQBaseAxis::setTitle(QString t) {
  title = t;
  updateGeometry();
}
