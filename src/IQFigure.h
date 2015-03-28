// IQFigure.h

#ifndef IQFIGURE_H

#define IQFIGURE_H

#include <QWidget>

class IQFigure: public QWidget {
  Q_OBJECT;
public:
  IQFigure(class IFigure *parent);
  virtual ~IQFigure();
  class QGridLayout *axesLayout() { return axlay; }
  class QHBoxLayout *buttonLayout() { return blay; }
protected:
  virtual void mousePressEvent(QMouseEvent *) override;
  virtual void mouseMoveEvent(QMouseEvent *) override;
  virtual void mouseReleaseEvent(QMouseEvent *) override;
  virtual void moveEvent(QMoveEvent *) override;
  virtual void resizeEvent(QResizeEvent *) override;
  virtual void closeEvent(QCloseEvent *) override;
private:
  class IFigure *parent;
  class QGridLayout *axlay;
  class QHBoxLayout *blay;
  class QVBoxLayout *lay;
};

#endif
