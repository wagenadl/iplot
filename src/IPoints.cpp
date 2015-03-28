// IPoints.cpp

#include "IPoints.h"
#include "IAxes.h"
#include <QColor>
#include <QPainter>
#include <QDebug>

IPoints::IPoints(IAxes *parent): IItem(parent) {
  props["type"] = "points";
  props["markersize"] = 6;
  props["markerstyle"] = ".";
}

IPoints::IPoints(IAxes *parent, IVector const &xx, IVector const &yy):
  IPoints(parent) {
  props["xdata"] = qVariantFromValue(xx);
  props["ydata"] = qVariantFromValue(yy);
  recalcBR();
}

IPoints::IPoints(IAxes *parent, QPointF const &xy):
  IPoints(parent) {
  IVector xx; xx << xy.x();
  IVector yy; yy << xy.y();
  props["xdata"] = qVariantFromValue(xx);
  props["ydata"] = qVariantFromValue(yy);
  recalcBR();
}

IPoints::~IPoints() {
}

bool IPoints::set(QString key, QVariant value) {
  bool res = IItem::set(key, value);
  if (key=="xdata" || key=="ydata")
    recalcBR();
  else
    parent->update();
  return res;
}

void IPoints::recalcBR() {
  double minx = 1e80;
  double maxx = -1e80;
  double miny = 1e80;
  double maxy = -1e80;
  for (double x: props["xdata"].value<IVector>()) {
    if (x<minx)
      minx = x;
    if (x>maxx)
      maxx = x;
  }

  for (double y: props["ydata"].value<IVector>()) {
    if (y<miny)
      miny = y;
    if (y>maxy)
      maxy = y;
  }

  if (minx==maxx) {
    minx -= 1e-10;
    maxx += 1e-10;
  }
  if (miny==maxy) {
    miny -= 1e-10;
    maxy += 1e-10;
  }

  QRectF b1(QPointF(minx, miny), QPointF(maxx, maxy));
  
  if (b1 != br) {
    br = b1;
    parent->updateLimits();
  }
}

QSet<QString> const &IPoints::properties() const {
  static QSet<QString> pp;
  if (pp.isEmpty()) {
    pp = IItem::properties();
    pp << "markersize";
    pp << "markerstyle"; // NYI
  }
  return pp;
  
}

void IPoints::paint(class QPainter &p) {
  QPen pen;
  pen.setColor(color());
  p.setPen(pen);
  QBrush b(color());;
  p.setBrush(b);
  double ms = getAsDouble("markersize");
  IVector xx = props["xdata"].value<IVector>();
  IVector yy = props["ydata"].value<IVector>();
  QPolygonF xy = parent->mapFromData(xx, yy);
  for (int k=0; k<xy.size(); k++) {
    p.drawEllipse(xy[k], ms, ms);
  }
}

static inline double length2(QPointF p) {
  double x = p.x();
  double y = p.y();
  return x*x + y*y;
}

static inline double length(QPointF p) {
  return sqrt(length2(p));
}

bool IPoints::hitTest(QPointF datac) const {
  double MARGIN = getAsDouble("markersize")/2.0 + 1;
  double MARGIN2 = MARGIN*MARGIN;
  IVector xx = props["xdata"].value<IVector>();
  IVector yy = props["ydata"].value<IVector>();
  QPolygonF pp = parent->mapFromData(xx, yy);
  QPointF p = parent->mapFromData(datac);
  // does p lie within MARGIN pixels of pp?

  int N = pp.size();
  for (int n=0; n<N; n++) {
    if (length2(p - pp[n])<MARGIN2)
      return true;
  }
  return false;
}

