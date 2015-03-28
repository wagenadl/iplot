// ILine.cpp

#include "ILine.h"
#include "IAxes.h"
#include <QColor>
#include <QPainter>
#include <QDebug>

ILine::ILine(IAxes *parent): IItem(parent) {
  props["type"] = "line";
  props["linewidth"] = 0;
  props["linestyle"] = "-";
}

ILine::ILine(IAxes *parent, IVector const &xx, IVector const &yy):
  ILine(parent) {
  props["xdata"] = qVariantFromValue(xx);
  props["ydata"] = qVariantFromValue(yy);
  recalcBR();
}


ILine::~ILine() {
}

bool ILine::set(QString key, QVariant value) {
  bool res = IItem::set(key, value);
  if (key=="xdata" || key=="ydata")
    recalcBR();
  else
    parent->update();
  return res;
}

void ILine::recalcBR() {
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
    maxx += 1e-10;
  }

  QRectF b1(QPointF(minx, miny), QPointF(maxx, maxy));

  if (b1 != br) {
    br = b1;
    parent->updateLimits();
  }
}

QSet<QString> const &ILine::properties() const {
  static QSet<QString> pp;
  if (pp.isEmpty()) {
    pp = IItem::properties();
    pp << "linewidth";
    pp << "linestyle";
  }
  return pp;
  
}

void ILine::paint(class QPainter &p) {
  QPen pen;
  pen.setWidthF(getAsDouble("linewidth"));
  pen.setColor(color());
  QString st = props["linestyle"].toString();
  pen.setStyle(st=="-" ? Qt::SolidLine
	       : st=="--" ? Qt::DashLine
	       : st=="." ? Qt::DotLine
	       : st==".-" ? Qt::DashDotLine
	       : Qt::SolidLine);
  p.setPen(pen);
  IVector xx = props["xdata"].value<IVector>();
  IVector yy = props["ydata"].value<IVector>();
  QPolygonF xy = parent->mapFromData(xx, yy);
  p.drawPolyline(xy);
}

static inline double length2(QPointF p) {
  double x = p.x();
  double y = p.y();
  return x*x + y*y;
}

static inline double length(QPointF p) {
  return sqrt(length2(p));
}

bool ILine::hitTest(QPointF datac) const {
  constexpr double MARGIN = 3;
  constexpr double MARGIN2 = MARGIN*MARGIN;
  IVector xx = props["xdata"].value<IVector>();
  IVector yy = props["ydata"].value<IVector>();
  QPolygonF pp = parent->mapFromData(xx, yy);
  QPointF p = parent->mapFromData(datac);
  // does p lie within MARGIN pixels of pp?

  int N = pp.size()-1;
  for (int n=0; n<N; n++) {
    QPointF p0 = pp[n];
    QPointF p1 = pp[n+1];
    QPointF d1 = p1 - p0;
    QPointF dp = p - p0;
    double l1 = length(d1);
    double lp = length(dp);
    if (l1<.1)
      l1 = .1;
    double t = lp/l1;
    QPointF pt = p0 + t*d1;
    if (length2(pt-p)<MARGIN2)
      return true;
  }
  return false;
}

