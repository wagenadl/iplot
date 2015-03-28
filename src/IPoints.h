// IPoints.h

#ifndef IPOINTS_H

#define IPOINTS_H

#include "IItem.h"
#include <QPolygonF>

class IPoints: public IItem {
public:
  IPoints(IAxes *parent);
  IPoints(IAxes *parent, class IVector const &xx, class IVector const &yy);
  IPoints(IAxes *parent, QPointF const &xy);
  virtual ~IPoints();
  virtual bool set(QString key, QVariant value) override;
  virtual QSet<QString> const &properties() const override;
  static IPoints *find(qulonglong handle) {
    return dynamic_cast<IPoints*>(IObject::find(handle)); }
  virtual void paint(class QPainter &) override;
  virtual QRectF boundingRect() const override { return br; }
  virtual bool hitTest(QPointF) const override;
private:
  void recalcBR();
private:
  QRectF br;
};

#endif
