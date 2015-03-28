// IQFrame.h

#ifndef IQFRAME_H

#define IQFRAME_H

#include <QFrame>

class IQFrame: public QFrame {
public:
  IQFrame(class IAxes *parent);
  class IQAxes *axes() { return w; }
  class IQXAxis *xAxis() { return xaxis; }
  class IQYAxis *yAxis() { return yaxis; }
protected:
  virtual void paintEvent(QPaintEvent *) override; 
protected:
  class IQAxes *w;
  class IQXAxis *xaxis;
  class IQYAxis *yaxis;

};

#endif
