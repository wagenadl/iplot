// IObject.cpp

#include "IObject.h"
#include <QDebug>
#include <QStringList>
#include "IVector.h"
#include "IFigure.h"
#include "IAxes.h"

static IHandle newHandle() {
  static IHandle h = 0;
  return ++h;
}

IObject::IObject(IObject *parent): QObject(parent), h(newHandle()) {
  props["parent"] = QVariant(parent->handle());
  props["type"] = "object";
  objects()[handle()] = this;
  props["root"] = parent ? parent->props["root"] : handle();
}

QString IObject::type() const {
  return props["type"].toString();
}

IObject::~IObject() {
  call("deletefcn", QVariant());
  objects().remove(handle());
}

bool IObject::set(QString key, QVariant value) {
  if (key=="children" || key=="parent" || key=="root")
    return false;
  if (hasProperty(key) || key.startsWith("*")) {
    props[key] = value;
    return true;
  } else {
    qDebug() << "No property " << key << " on object of type " << type();
    QStringList pp;
    for (auto p: properties())
      pp << p;
    qDebug() << "Properties are" << pp.join(" ");
    return false;
  }
}

QVariant IObject::get(QString key) const {
  if (key=="children")
    return qVariantFromValue(children());
  else if (props.contains(key))
    return props[key];
  else if (properties().contains(key))
    return QVariant();

  qDebug() << "No property " << key << " on object of type " << type();
  QStringList pp;
  for (auto p: properties())
    pp << p;
  qDebug() << "Properties are" << pp.join(" ");
  return QVariant();
}

void IObject::setCallbackHandler(CallbackHandler *c) {
  cbh() = c;
}

bool IObject::call(QString fcnkey, QVariant arg) {
  if (!cbh())
    return false;
  QVariant v = get(fcnkey);
  bool b = false;
  if (v.type()==QVariant::UserType) {
    IVector iv = v.value<IVector>();
    if (!iv.isEmpty())
      b = iv[0]>0;
  } else {
    b = v.toInt()>0;
  }
  if (b) {
    cbh()->activate(handle(), fcnkey, arg);
    return true;
  } else {
    return false;
  }
}
  
bool IObject::hasProperty(QString key) {	
  return properties().contains(key) || props.contains(key);
}

QSet<QString> IObject::allProperties() const {
  QSet<QString> pp = properties();
  pp |= QSet<QString>::fromList(props.keys());
  return pp;
}

QSet<QString> const &IObject::properties() const {
  static QSet<QString> pp;
  if (pp.isEmpty()) {
    pp << "children";
    pp << "parent";
    pp << "type";
    pp << "deletefcn";
    pp << "tag";
  }
  return pp;
}

IHandle IObject::handle() const {
  if (this)
    return h;
  else
    return 0;
}

IHandle IObject::findByTag(QString tag) {
  for (IObject const *obj: objects())
    if (obj->get("tag").toString()==tag)
      return obj->handle();
  return 0;
}

IObject *IObject::find(IHandle handle) {
  if (objects().contains(handle))
    return objects()[handle];
  else
    return NULL;
}

QMap<IHandle, QPointer<IObject> > &IObject::objects() {
  static QMap<IHandle, QPointer<IObject> > map;
  return map;
}

IHandleSet IObject::children() const {
  IHandleSet res;
  for (auto c: QObject::children()) {
    IObject *ch = dynamic_cast<IObject *>(c);
    if (ch)
      res << ch->handle();
  }
  return res;
}

IHandle IObject::findChildByTag(QString tag) const {
  for (auto c: QObject::children()) {
    IObject *ch = dynamic_cast<IObject *>(c);
    if (ch) {
      if (ch->get("tag").toString()==tag)
	return ch->handle();
      IHandle h = ch->findChildByTag(tag);
      if (h)
	return h;
    }
  }
  return 0;
}


IHandle &IObject::gca() {
  static IHandle h = 0;
  if (!IAxes::find(h))
    h = 0;
  return h;
}

IHandle &IObject::gcf() {
  static IHandle h = 0;
  if (!IFigure::find(h))
    h = 0;
  return h;
}

IHandle &IObject::gcbo() {
  static IHandle h = 0;
  if (!find(h))
    h = 0;
  return h;
}

IHandle IObject::gcbf() {
  IObject *obj = find(gcbo());
  if (obj)
    return obj->get("root").toULongLong();
  else
    return 0;
}

IHandle IObject::parent() const {
  // not valid on null objects
  IObject const *p = dynamic_cast<IObject const *>(QObject::parent());
  return p->handle();
}

QPointer<CallbackHandler> &IObject::cbh() {
  static QPointer<CallbackHandler> c;
  return c;
}

double IObject::getAsDouble(QString key) const {
  bool ok;
  QVariant v = get(key);
  double x = v.toDouble(&ok);
  if (ok)
    return x;
  IVector xx = v.value<IVector>();
  if (!xx.isEmpty())
    return xx[0];
  return nan("");
}

QSizeF IObject::getAsSizeF(QString key) const {
  QVariant v = get(key);
  if (v.canConvert(QVariant::SizeF))
    return v.toSizeF();
  IVector xx = v.value<IVector>();
  if (xx.size()==2)
    return QSizeF(xx[0], xx[1]);
  return QSizeF();
}
  
QPointF IObject::getAsPointF(QString key) const {
  QVariant v = get(key);
  if (v.canConvert(QVariant::PointF))
    return v.toPointF();
  IVector xx = v.value<IVector>();
  if (xx.size()==2)
    return QPointF(xx[0], xx[1]);
  return QPointF();
}
  
