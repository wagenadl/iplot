// IObject.h

#ifndef IOBJECT_H

#define IOBJECT_H

#include <QObject>
#include <QVariant>
#include <QSet>
#include <QPointer>
#include <QPointF>
#include "CallbackHandler.h"
#include "IHandle.h"

class IObject: public QObject {
  Q_OBJECT;
public:
  IObject(IObject *parent=0);
  virtual ~IObject();
  virtual bool set(QString key, QVariant value);
  virtual QVariant get(QString key) const;
  bool hasProperty(QString key); // predeclared or user-defined
  virtual QSet<QString> const &properties() const; // only predeclared props
  QSet<QString> allProperties() const; // also user-defined properties
  bool call(QString fcnkey, QVariant arg);
  IHandle handle() const; // Defined even if this==null.
  static IObject *find(IHandle);
  QString type() const;
  IHandleSet children() const;
  IHandle parent() const;
  IHandle findChildByTag(QString tag) const;
  static IHandle findByTag(QString tag);
  static IHandle &gca();
  static IHandle &gcf();
  static IHandle &gcbo();
  static IHandle gcbf();
  static void setCallbackHandler(CallbackHandler *cbh);
  double getAsDouble(QString key) const;
  QPointF getAsPointF(QString key) const;
  QSizeF getAsSizeF(QString key) const;
protected:
  QVariantMap props;
  static QMap<IHandle, QPointer<IObject> > &objects();
  static QPointer<CallbackHandler> &cbh();
private:
  IHandle h;
};

#endif
