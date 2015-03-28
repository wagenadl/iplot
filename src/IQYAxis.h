// IQYaxis.h

#ifndef IQYAXIS_H

#define IQYAXIS_H

#include "IQBaseAxis.h"

class IQYAxis: public IQBaseAxis {
public:
  IQYAxis(QWidget *parent=0);
  virtual QSize sizeHint() const;
  virtual void paint(class QPainter &);
};

#endif
