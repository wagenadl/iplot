// CallbackHandler.cpp

#include "CallbackHandler.h"

CallbackHandler::CallbackHandler(QObject *parent): QObject(parent) {
}

void CallbackHandler::activate(IHandle h, QString k, QVariant arg) {
  emit activated(h, k, arg);
}
