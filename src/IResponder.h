// IResponder.h

#ifndef IRESPONDER_H

#define IRESPONDER_H

#include "IObject.h"
#include "Command.h"
#include "Responder.h"

class IResponder: public QObject {
  Q_OBJECT;
public:
  IResponder(class Responder *communicator);
public slots:
  void respond();
  void callback(IHandle h, QString key, QVariant arg);
private:
  void cFigure();
  void cClose();
  void cAxes();
  void cItem();
  void cGCx();
  void cSubplot();
  void cGet();
  void cSet();
  void cProperties();
  void cFind();
  void cDelete();
  void cText();
  void cButton();
  void sendHandle(IHandle h);
  void sendResponse(Command::Command cmd, IHandle h=0);
  void sendCallbackOrValue(Responder::Header hdr, QString key, QVariant arg);
private:
  class Responder *com;
};

#endif
