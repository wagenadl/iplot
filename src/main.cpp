// main.cpp

#include <QApplication>
#include "IResponder.h"
#include "Responder.h"
#include <QDebug>
#include <stdio.h>

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(false);
  Responder resp; 
  IResponder iresp(&resp);
  return app.exec();
}

  

