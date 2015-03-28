// IPatch.cpp

#include "IPatch.h"
#include "IAxes.h"
#include <QColor>
#include <QPainter>
#include <QDebug>

IPatch::IPatch(IAxes *parent): ILine(parent) {
  props["type"] = "patch";
  props.remove("linewidth");
  props.remove("linestyle");
}

IPatch::IPatch(IAxes *parent, IVector const &xx, IVector const &yy):
  IPatch(parent) {
  props["xdata"] = qVariantFromValue(xx);
  props["ydata"] = qVariantFromValue(yy);
  recalcBR();
}


IPatch::~IPatch() {
}

QSet<QString> const &IPatch::properties() const {
  static QSet<QString> pp;
  if (pp.isEmpty()) {
    pp = IItem::properties();
    // note that we are skipping the line-specific ones
  }
  return pp;
  
}

void IPatch::paint(class QPainter &p) {
  QPen pen(Qt::NoPen);
  QBrush br(color());
  p.setPen(pen);
  p.setBrush(br);
  IVector xx = props["xdata"].value<IVector>();
  IVector yy = props["ydata"].value<IVector>();
  QPolygonF xy = parent->mapFromData(xx, yy);
  p.drawPolygon(xy);
}

bool IPatch::hitTest(QPointF datac) const {
  IVector xx = props["xdata"].value<IVector>();
  IVector yy = props["ydata"].value<IVector>();
  QPolygonF xy(xx.size());
  for (int n=0; n<xx.size(); n++)
    xy[n] = QPointF(xx[n], yy[n]);
  return xy.contains(datac);
}

