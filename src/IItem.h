// IItem.h

#ifndef IITEM_H

#define IITEM_H

#include "IObject.h"
#include <QPointF>
#include <QRectF>
#include "IVector.h"

class IItem: public IObject {
public:
  IItem(class IAxes *parent);
  virtual ~IItem() { }
  virtual QRectF boundingRect() const { return QRectF(); } // in data coords
  // "top" will be the minimum y, which is really at the bottom.
  virtual bool hitTest(QPointF) const { return false; } // data coords
  virtual void paint(class QPainter &) { }
  static IItem *find(IHandle handle) {
    return dynamic_cast<IItem*>(IObject::find(handle)); }
  virtual QSet<QString> const &properties() const override;
  QColor color() const;
protected:
  IAxes *parent;
};

#endif
