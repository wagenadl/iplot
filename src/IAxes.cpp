// IAxes.cpp

#include "IAxes.h"
#include "IFigure.h"
#include "IQAxes.h"
#include "IQFrame.h"
#include "IQXAxis.h"
#include "IQYAxis.h"
#include "IQFigure.h"
#include "IItem.h"
#include <QDebug>
#include <QFrame>

IAxes::IAxes(IFigure *parent): IObject(parent) {
  props["type"] = "axes";
  props["xlim"] = QPoint(0, 1);
  props["ylim"] = QPoint(0, 1);
  props["xlimmode"] = "auto";
  props["ylimmode"] = "auto";
  frame = new IQFrame(this);
}

IAxes::~IAxes() {
  // Our widgets will be deleted by the containing layout unless we are
  // explicitly deleted, in which case we must do it ourselves
  delete frame;
}

bool IAxes::set(QString key, QVariant value) {
  if (key=="xlimmode" || key=="ylimmode") {
    QString v = value.toString();
    if (v!="auto" && v!="manual")
      return false;
  }
  bool res = IObject::set(key, value);
  if (key=="xlimmode" || key=="ylimmode") {
    updateLimits();
  } else {
    if (key=="xlim") {
      frame->xAxis()->setLimits(getAsPointF("xlim"));
      props["xlimmode"] = QString("manual");
    } else if (key=="ylim") {
      frame->yAxis()->setLimits(getAsPointF("ylim"));
      props["ylimmode"] = QString("manual");
    } else if (key=="xlabel") {
      frame->xAxis()->setTitle(value.toString());
    } else if (key=="ylabel") {
      frame->yAxis()->setTitle(value.toString());
    }
    update();
  }
  return res;
}

QSet<QString> const &IAxes::properties() const {
  static QSet<QString> pp;
  if (pp.isEmpty()) {
    pp = IObject::properties();
    pp << "position";
    pp << "size";
    pp << "buttondownfcn";
    pp << "buttonmotionfcn";
    pp << "buttonupfcn";
    pp << "currentpoint";
    pp << "downpoint";
    pp << "xlim";
    pp << "ylim";
    pp << "xlimmode";
    pp << "ylimmode";
    pp << "xlabel";
    pp << "ylabel";
  }
  return pp;
}

void IAxes::update() {
  frame->update();
}

void IAxes::updateLimits() {
  if (get("xlimmode")==QVariant("auto")) {
    double min = 1e80;
    double max = -1e80;
    for (auto c: children()) {
      IItem *i = IItem::find(c);
      if (i) {
	QRectF r = i->boundingRect();
	if (r.left()<min)
	  min = r.left();
	if (r.right()>max)
	  max = r.right();
      }
    }
    if (min==max) {
      min -= .1;
      max += .1;
    }
    props["xlim"] = QPointF(min, max);
  }
  if (get("ylimmode")==QVariant("auto")) {
    double min = 1e80;
    double max = -1e80;
    for (auto c: children()) {
      IItem *i = IItem::find(c);
      if (i) {
	QRectF r = i->boundingRect();
	if (r.top()<min)
	  min = r.top();
	if (r.bottom()>max)
	  max = r.bottom();
      }
    }
    if (min==max) {
      min -= .1;
      max += .1;
    }
    props["ylim"] = QPointF(min, max);
  }
  frame->xAxis()->setLimits(getAsPointF("xlim"));
  frame->yAxis()->setLimits(getAsPointF("ylim"));
  frame->update();
}

QPointF IAxes::mapToData(QPointF widgetcoords) const {
  QRectF r0 = frame->axes()->contentsRect();
  QPointF xlim = getAsPointF("xlim"); // note: really a range, not a point
  QPointF ylim = getAsPointF("ylim"); // ditto
  double rx = (widgetcoords.x() - r0.left()) / r0.width();
  double ry = (widgetcoords.y() - r0.top()) / r0.height();
  double x = xlim.x() + rx*(xlim.y()-xlim.x());
  double y = ylim.y() - ry*(ylim.y()-ylim.x());
  return QPointF(x, y);
}

QPointF IAxes::mapFromData(QPointF datacoords) const {
  QRectF r0 = frame->axes()->contentsRect();
  QPointF xlim = getAsPointF("xlim"); // note: really a range, not a point
  QPointF ylim = getAsPointF("ylim"); // ditto
  double rx = (datacoords.x() - xlim.x()) / (xlim.y()-xlim.x());
  double ry = (datacoords.y() - ylim.x()) / (ylim.y()-ylim.x());
  double x = r0.left() + r0.width()*rx;
  double y = r0.bottom() - r0.height()*ry;
  return QPointF(x, y);
}

QPolygonF IAxes::mapFromData(IVector const &xx, IVector const &yy) const {
  if (xx.size() != yy.size())
    return QPolygonF();
  
  QRectF r0 = frame->axes()->contentsRect();
  QPointF xlim = getAsPointF("xlim"); // note: really a range, not a point
  QPointF ylim = getAsPointF("ylim"); // ditto

  int N = xx.size();
  QPolygonF xy(N);
  double x0 = xlim.x();
  double y0 = ylim.x();
  double dx = xlim.y() - xlim.x();
  double dy = ylim.y() - ylim.x();
  double w = r0.width();
  double h = r0.height();
  double xl = r0.left();
  double yb = r0.bottom();

  for (int n=0; n<N; n++) {
    double rx = (xx[n] - x0)/dx;
    double ry = (yy[n] - y0)/dy;
    double x = xl + w*rx;
    double y = yb - h*ry;
    xy[n] = QPointF(x, y);
  }

  return xy;
}

void IAxes::mouseEvent(QString type, QPointF widgetc, int button) {
  QPointF datac = mapToData(widgetc);
  QString fcn = "button" + type + "fcn";
  IVector v; v << datac.x() << datac.y();
  props["currentpoint"] = qVariantFromValue(v);
  gca() = handle();
  gcf() = parent();
  
  if (type=="down") {
    props["downpoint"] = qVariantFromValue(v);
    gcbo() = handle();
    QList<IHandle> cc;
    for (auto c: children())
      cc.push_front(c);
    for (auto c: cc) { // inspect in reverse creation order
      IItem *i = IItem::find(c);
      if (i && i->hitTest(datac)) {
	gcbo() = i->handle();
	break;
      }
    }
  }
  if (gcbo()==handle()) {
    call(fcn, button);
  } else {
    IItem *i = IItem::find(gcbo());
    if (i)    
      i->call(fcn, button);
  }
}

void IAxes::paint(QPainter &p) {
  for (auto c: children()) {
    IItem *i = IItem::find(c);
    if (i)
      i->paint(p);
  }
}

IAxes *IAxes::ensureGCA() {
  IAxes *a = find(gca());
  if (a)
    return a;

  IFigure *f = IFigure::ensureGCF();
  IHandle h = f->currentAxes();
  a = find(h);
  if (!a)
    a = f->subplot(0);
  gca() = a->handle();
  return a;
}
