// IQAxes.h

#ifndef IQAXES_H

#define IQAXES_H

#include <QWidget>

class IQAxes: public QWidget {
public:
  IQAxes(class IAxes *parent);
  virtual ~IQAxes();
  void paint(class QPainter &);
protected:
  virtual void mousePressEvent(QMouseEvent *) override;
  virtual void mouseMoveEvent(QMouseEvent *) override;
  virtual void mouseReleaseEvent(QMouseEvent *) override;
private:
  class IAxes *parent;
};

#endif
