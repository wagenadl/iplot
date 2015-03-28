// ILine.h

#ifndef ILINE_H

#define ILINE_H

#include "IItem.h"
#include <QPolygonF>

class ILine: public IItem {
public:
  ILine(IAxes *parent);
  ILine(IAxes *parent, class IVector const &xx, class IVector const &yy);
  virtual ~ILine();
  virtual bool set(QString key, QVariant value) override;
  virtual QSet<QString> const &properties() const override;
  static ILine *find(qulonglong handle) {
    return dynamic_cast<ILine*>(IObject::find(handle)); }
  virtual void paint(class QPainter &) override;
  virtual QRectF boundingRect() const override { return br; }
  virtual bool hitTest(QPointF) const override;
protected:
  void recalcBR();
protected:
  QRectF br;
};

#endif
