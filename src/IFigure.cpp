// IFigure.cpp

#include "IFigure.h"
#include "IQFigure.h"
#include "IAxes.h"
#include <QGridLayout>
#include "IQAxes.h"
#include <QDebug>
#include "IButton.h"

uint qHash(QPoint p) {
  return qHash(p.x() ^ qHash(p.y()));
}

IFigure::IFigure(IObject *parent): IObject(parent) {
  props["type"] = "figure";
  props["deletefcn"] = 1;
  nrows = ncolumns = 1;
  gca_ = 0;
  w = new IQFigure(this);
  set("size", QSize(800, 600));
  w->show();
}

IFigure::IFigure(QSize size, IObject *parent): IFigure(parent) {
  set("size", size);
}

IFigure::IFigure(QRect geom, IObject *parent): IFigure(parent) {
  set("position", geom.topLeft());
  set("size", geom.size());
}

IFigure::~IFigure() {
  delete w;
}

bool IFigure::set(QString key, QVariant value) {
  bool res = IObject::set(key, value);
  if (key=="position") 
    w->move(getAsPointF(key).toPoint());
  else if (key=="size")
    w->resize(getAsSizeF(key).toSize());
  else if (key=="title")
    w->setWindowTitle(value.toString());
  return res;
}

QSet<QString> const &IFigure::properties() const {
  static QSet<QString> pp;
  if (pp.isEmpty()) {
    pp = IObject::properties();
    pp << "position";
    pp << "size";
    pp << "title";
    //pp << "windowbuttondownfcn";
    //pp << "windowbuttonmotionfcn";
    //pp << "windowbuttonupfcn";
    //pp << "currentpoint";
  }
  return pp;
}

IAxes *IFigure::subplot(int rows, int columns, int row, int col) {
  if (rows!=nrows || columns!=ncolumns) {
    for (auto a: axes) 
      delete IAxes::find(a);
    axes.clear();
    nrows = rows;
    ncolumns = columns;
  }
  return subplot(row, col);
}
    
IAxes *IFigure::subplot(int row, int col) {
  if (axes.contains(QPoint(col, row))) {
    IAxes *ax = IAxes::find(axes[QPoint(col, row)]);
    if (ax) {
      selectAxes(ax->handle());
      return ax;
    }
  }
  IAxes *ax = new IAxes(this);
  w->axesLayout()->addWidget(ax->widget(), row, col);
  ax->set("position", QPointF(col/double(ncolumns), row/double(nrows)));
  ax->set("size", QPointF(1/double(ncolumns), 1/double(nrows)));
  axes[QPoint(col, row)] = ax->handle();
  selectAxes(ax->handle());
  return ax;
}

IAxes *IFigure::subplot(int rows, int columns, int n) {
  return subplot(rows, columns, n/columns, n%columns);
}

IAxes *IFigure::subplot(int n) {
  return subplot(n/ncolumns, n%ncolumns);
}

void IFigure::updateGeometry() {
  props["position"] = w->pos();
  props["size"] = w->size();
}

IFigure *IFigure::ensureGCF() {
  IFigure *f = find(gcf());
  if (f)
    return f;
  
  f = new IFigure();
  gcf() = f->handle();
  return f;
}    

IHandleSet IFigure::all() {
  IHandleSet hh;
  for (IObject const *obj: objects())
    if (dynamic_cast<IFigure const*>(obj))
      hh << obj->handle();
  return hh;
}

IHandle IFigure::currentAxes() {
  if (!IAxes::find(gca_))
    gca_ = 0;
  return gca_;
}
    
void IFigure::selectAxes(IHandle a) {
  gca_ = a;
}

IButton *IFigure::newButton(QString typ, QString txt) {
  if (!IButton::types().contains(typ))
    return NULL;

  IButton *but = new IButton(this, typ, txt);
  w->buttonLayout()->insertWidget(w->buttonLayout()->count()-1, but->widget());
  return but;
}
