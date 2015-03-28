// IQXAxis.h

#ifndef IQXAXIS_H

#define IQXAXIS_H

#include "IQBaseAxis.h"

class IQXAxis: public IQBaseAxis {
public:
  IQXAxis(QWidget *parent=0);
  virtual QSize sizeHint() const;
  virtual void paint(class QPainter &);
};

#endif
