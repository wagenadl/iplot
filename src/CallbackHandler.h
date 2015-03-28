// CallbackHanler.h

#ifndef CALLBACKHANLER_H

#define CALLBACKHANLER_H

#include "IHandle.h"

class CallbackHandler: public QObject {
  Q_OBJECT;
public:
  CallbackHandler(QObject *parent=0);
  virtual ~CallbackHandler() { }
public slots:
  void activate(IHandle h, QString key, QVariant arg);
signals:
  void activated(IHandle h, QString key, QVariant arg);
};

#endif
