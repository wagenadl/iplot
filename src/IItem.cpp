// IItem.cpp

#include "IItem.h"
#include "IAxes.h"
#include <QColor>

IItem::IItem(IAxes *parent): IObject(parent), parent(parent) {
  props["type"] = "item";
  IVector cc; cc << 0 << 0 << 1;
  props["color"] = qVariantFromValue(cc);
}

QColor IItem::color() const {
  QVariant c = props["color"];
  if (c.canConvert(QVariant::Color))
    return c.value<QColor>();
  IVector v = c.value<IVector>();
  if (v.size()==3)
    return QColor(v[0]*255, v[1]*255, v[2]*255);
  return QColor(0, 0, 0);
}

QSet<QString> const &IItem::properties() const {
  static QSet<QString> pp;
  if (pp.isEmpty()) {
    pp = IObject::properties();
    pp << "xdata";
    pp << "ydata";
    pp << "color";
    pp << "buttondownfcn";
    pp << "buttonmotionfcn";
    pp << "buttonupfcn";
  }
  return pp;
}
