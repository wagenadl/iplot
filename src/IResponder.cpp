// IResponder.cpp

#include "IResponder.h"
#include <QSocketNotifier>
#include "IFigure.h"
#include "IAxes.h"
#include "ILine.h"
#include "IPoints.h"
#include "IPatch.h"
#include "IText.h"
#include "IButton.h"

#include <QApplication>
#include "Responder.h"
#include <QDebug>
#include "Command.h"
#include "CallbackHandler.h"

IResponder::IResponder(class Responder *com):
  com(com) {
  QSocketNotifier *sn
    = new QSocketNotifier(com->recvfd(), QSocketNotifier::Read, this);
  connect(sn, SIGNAL(activated(int)), SLOT(respond()));

  CallbackHandler *cbh = new CallbackHandler(this);
  IObject::setCallbackHandler(cbh);
  connect(cbh, SIGNAL(activated(IHandle, QString, QVariant)),
          SLOT(callback(IHandle, QString, QVariant)));
}

void IResponder::respond() {
  if (!com->receive()) {
    if (com->closed()) 
      qDebug() << "IPlot server: IResponder: Client closed us off";
    else
      qDebug() << "IPlot server: IResponder: No message!?";
    QApplication::quit();
    return;
  }

  Responder::Header const &hdr = com->receivedHeader();
  switch ((Command::Command)hdr.cmd) {
  case Command::Figure: cFigure(); break;
  case Command::Close: cClose(); break;
  case Command::Axes: cAxes(); break;
  case Command::Line: case Command::Points: case Command::Patch: cItem(); break;
  case Command::Text: cText(); break;
  case Command::Button: cButton(); break;
  case Command::Subplot: cSubplot(); break;
  case Command::Get: cGet(); break;
  case Command::Set: cSet(); break;
  case Command::Properties: cProperties(); break;
  case Command::Find: cFind(); break;
  case Command::Delete: cDelete(); break;
  case Command::GCF: case Command::GCA: case Command::GCBO: case Command::GCBF:
    cGCx(); break;
  default: sendResponse(Command::BadCommand);
  }
};

void IResponder::sendHandle(IHandle h) {
  sendResponse(Command::Handle, h);
}

void IResponder::sendResponse(Command::Command cmd, IHandle h) {
  Responder::Header hdr;
  Responder::prepareHeader(&hdr);
  hdr.seqid = com->receivedHeader().seqid;
  hdr.cmd = cmd;
  hdr.handle = h;
  if (!com->send(&hdr)) {
    qDebug() << "IPlot server: Failed to send response";
    QApplication::quit();
  }
}

void IResponder::cClose() {
  IHandle h = (IHandle)com->receivedHeader().handle;
  if (h==0) {
    IHandleSet hh = IFigure::all();
    for (auto h: hh) 
      delete IFigure::find(h);
    sendResponse(Command::OK);
  } else {
    IFigure *fig = IFigure::find(h);
    if (fig) {
      delete fig;
      sendResponse(Command::OK);
    } else {
      sendResponse(Command::BadHandle);
    }
  }
}

void IResponder::cFigure() {
  IObject::gcf() = 0;
  IObject::gca() = 0;
  Responder::Header const &hdr = com->receivedHeader();
  IHandle h = (IHandle)hdr.handle;
  if (h) {
    IFigure *fig = IFigure::find(h);
    if (fig) {
      IObject::gcf() = h;
    } 
  } else {
    if (hdr.nargs>0) {
      int w = com->retrieveInt(0, 0);
      int h = com->retrieveInt(0, 1);
      if (w>0 && h>0) {
	IFigure *fig = new IFigure(QSize(w, h));
	IObject::gcf() = fig->handle();
      }
    } else {
      IFigure *fig = new IFigure();
      IObject::gcf() = fig->handle();
    }
  }
  sendHandle(IObject::gcf());
}

void IResponder::cAxes() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs!=1) {
    sendResponse(Command::BadSyntax);
    return;
  }
  IHandle h = (IHandle)hdr.handle;
  if (h) {
    IAxes *ax = IAxes::find(h);
    if (!ax) {
      sendResponse(Command::BadHandle);
      return;
    }
    IObject::gca() = ax->handle();
    IObject::gcf() = ax->parent();
  } else {
    sendResponse(Command::BadHandle);
    return;
  }
  sendResponse(Command::OK);
}

void IResponder::cSubplot() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs!=1) {
    sendResponse(Command::BadSyntax);
    return;
  }
  IFigure *fig = IFigure::ensureGCF();
  
  int N = com->retrieveCount(0);
  IAxes *ax = (N==1) ? fig->subplot(com->retrieveInt(0, 0))
    : (N==2) ? fig->subplot(com->retrieveInt(0, 0), com->retrieveInt(0, 1))
    : (N==3) ? fig->subplot(com->retrieveInt(0, 0), com->retrieveInt(0, 1),
			    com->retrieveInt(0, 2))
    : (N==4) ? fig->subplot(com->retrieveInt(0, 0), com->retrieveInt(0, 1),
			    com->retrieveInt(0, 2), com->retrieveInt(0, 3))
    : 0;
  IObject::gca() = ax->handle();
  sendHandle(IObject::gca());
}

static IItem *makeItem(Command::Command c, IAxes *ax) {
  switch (c) {
  case Command::Line: return new ILine(ax);
  case Command::Points: return new IPoints(ax);
  case Command::Patch: return new IPatch(ax);
  default: return NULL;
  }
}

static IItem *makeItem(Command::Command c, IAxes *ax,
                       IVector const &xx, IVector const &yy) {
  switch (c) {
  case Command::Line: return new ILine(ax, xx, yy);
  case Command::Points: return new IPoints(ax, xx, yy);
  case Command::Patch: return new IPatch(ax, xx, yy);
  default: return NULL;
  }
}

void IResponder::cGCx() {
  switch ((Command::Command)com->receivedHeader().cmd) {
  case Command::GCF: sendHandle(IFigure::ensureGCF()->handle()); break;
  case Command::GCA: sendHandle(IAxes::ensureGCA()->handle()); break;
  case Command::GCBO: sendHandle(IObject::gcbo()); break;
  case Command::GCBF: sendHandle(IObject::gcbf()); break;
  default: sendResponse(Command::BadCommand); break;
  }
}

void IResponder::cText() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs<2) {
    sendResponse(Command::BadSyntax);
    return;
  }
  if (hdr.argtype[1]!=Responder::DT_Char) {
    sendResponse(Command::BadValue);
    return;
  }

  IVector pos; pos.resize(2);
  if (!com->retrieveDoubles(0, pos.data())) {
    sendResponse(Command::BadValue);
    return;
  }
  QPointF p(pos[0], pos[1]);

  QString txt(QByteArray((char const *)com->receivedArg(1), hdr.arglen[1]));

  IAxes *ax = IAxes::ensureGCA();
  IText *it = new IText(ax, p, txt);
  sendHandle(it->handle());
}

void IResponder::cButton() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs<2) {
    sendResponse(Command::BadSyntax);
    return;
  }

  QString typ(QByteArray((char const *)com->receivedArg(0), hdr.arglen[0]));
  QString txt(QByteArray((char const *)com->receivedArg(1), hdr.arglen[1]));

  IFigure *fig = IFigure::ensureGCF();
  IButton *ib = fig->newButton(typ, txt);
  if (ib)
    sendHandle(ib->handle());
  else 
    sendResponse(Command::BadValue);
}

void IResponder::cItem() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs<2) {
    sendResponse(Command::BadSyntax);
    return;
  }
  IAxes *ax = IAxes::ensureGCA();
  int N = com->retrieveCount(0);
  int M = com->retrieveCount(1);
  if (M!=N) {
    sendResponse(Command::BadValue);
    return;
  }

  if (N==0) {
    IItem *it = makeItem((Command::Command)hdr.cmd, ax);
    if (it)
      sendHandle(it->handle());
    else
      sendResponse(Command::BadCommand);
    return;
  }
  
  IVector xx; xx.resize(N);
  if (!com->retrieveDoubles(0, xx.data())) {
    sendResponse(Command::BadValue);
    return;
  }
  
  IVector yy; yy.resize(N);
  if (!com->retrieveDoubles(1, yy.data())) {
    sendResponse(Command::BadValue);
    return;
  }

  IItem *it = makeItem((Command::Command)hdr.cmd, ax, xx, yy);
  if (it)
    sendHandle(it->handle());
  else
    sendResponse(Command::BadCommand);
}

template <typename T> IVector extractVector(void const *arg, int L) {
  int N = L / sizeof(T);
  IVector res; res.resize(N);
  T const *src = (T const *)arg;
  for (int n=0; n<N; n++)
    res[n] = src[n];
  return res;
}

void IResponder::cFind() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs!=1) {
    sendResponse(Command::BadSyntax);
    return;
  }
  if (hdr.argtype[0]!=Responder::DT_Char) {
    sendResponse(Command::BadKey);
    return;
  }
  QString tag(QByteArray((char const *)com->receivedArg(0), hdr.arglen[0]));
  if (hdr.handle==0) {
    IHandle h = IObject::findByTag(tag);
    sendHandle(h);
  } else {
    IObject *obj = IObject::find(hdr.handle);
    if (obj) {
      IHandle h = obj->findChildByTag(tag);
      sendHandle(h);
    } else {
      sendResponse(Command::BadHandle);
    }
  }
}

void IResponder::cSet() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs!=2) {
    sendResponse(Command::BadSyntax);
    return;
  }
  if (hdr.argtype[0]!=Responder::DT_Char) {
    sendResponse(Command::BadKey);
    return;
  }
    
  IObject *obj = IObject::find(hdr.handle);
  if (!obj) {
    sendResponse(Command::BadHandle);
    return;
  }
  
  QByteArray key0((char const *)com->receivedArg(0), hdr.arglen[0]);
  QString key(key0);

  if (key=="parent" || key=="children") {
    sendResponse(Command::BadKey);
    return;
  }
   
  QVariant val;
  void const *arg = com->receivedArg(1);
  int L = hdr.arglen[1];
  switch ((Responder::DataType)(hdr.argtype[1])) {
  case Responder::DT_Char:
    val = QString(QByteArray((char const *)arg, L));
    break;
  case Responder::DT_Int32:
    val = qVariantFromValue(extractVector<int32_t>(arg, L));
    break;
  case Responder::DT_UInt64:
    val = qVariantFromValue(extractVector<uint64_t>(arg, L));
    break;
  case Responder::DT_Float:
    val = qVariantFromValue(extractVector<float>(arg, L));
    break;
  case Responder::DT_Double:
    val = qVariantFromValue(extractVector<double>(arg, L));
    break;
  default:
    sendResponse(Command::BadValue);
    return;
  }

  if (obj->set(key, val)) 
    sendResponse(Command::OK);
  else if (obj->properties().contains(key))
    sendResponse(Command::BadValue);
  else
    sendResponse(Command::BadKey);
}


void IResponder::cDelete() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs!=0) {
    sendResponse(Command::BadSyntax);
    return;
  }
  IObject *obj = IObject::find(hdr.handle);
  if (obj) {
    delete obj;
    sendResponse(Command::OK);
  } else {
    sendResponse(Command::BadHandle);
  }
}

void IResponder::cProperties() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs!=0) {
    sendResponse(Command::BadSyntax);
    return;
  }
  IObject *obj = IObject::find(hdr.handle);
  if (!obj) {
    sendResponse(Command::BadHandle);
    return;
  }

  QSet<QString> pp = obj->allProperties();
  QString res = QStringList(pp.toList()).join(" ");
  QByteArray ar = res.toUtf8();

  Responder::Header out;
  Responder::prepareHeader(&out);
  out.seqid = hdr.seqid;
  out.handle = hdr.handle;
  out.cmd = Command::Value;
  out.nargs = 2;
  out.argtype[0] = Responder::DT_Char;
  out.arglen[0] = 0;
  out.argtype[1] = Responder::DT_Char;
  out.arglen[1] = ar.size();
  if (!com->send(&out, 0, ar.data())) {
    qDebug() << "IPlot server: Failed to send response";
    QApplication::quit();
  }
}

void IResponder::cGet() {
  Responder::Header const &hdr = com->receivedHeader();
  if (hdr.nargs!=1) {
    sendResponse(Command::BadSyntax);
    return;
  }
  if (hdr.argtype[0]!=Responder::DT_Char) {
    sendResponse(Command::BadKey);
    return;
  }
    
  IObject *obj = IObject::find(hdr.handle);
  if (!obj) {
    sendResponse(Command::BadHandle);
    return;
  }
  
  QByteArray key0((char const *)com->receivedArg(0), hdr.arglen[0]);
  QString key(key0);

  Responder::Header out;
  Responder::prepareHeader(&out);
  out.seqid = hdr.seqid;
  out.handle = hdr.handle;
  out.cmd = Command::Value;
  
  QVariant val = obj->get(key);
  if (val.isNull() && !obj->properties().contains(key)) {
    // invalid null
    sendResponse(Command::BadKey);
    return;
  }

  sendCallbackOrValue(out, key, val);
}

void IResponder::sendCallbackOrValue(Responder::Header out,
                                     QString key, QVariant val) {
  QByteArray k = key.toUtf8();
  out.arglen[0] = k.size();
  out.argtype[0] =  Responder::DT_Char;
  if (val.isNull()) {
    out.nargs = 1;
    com->send(&out, k.data());
    return;
  }
  
  out.nargs = 2;
  switch (val.type()) {
  case QVariant::String: {
    QByteArray ar = val.toString().toUtf8();
    out.argtype[1] = Responder::DT_Char;
    out.arglen[1] = ar.size();
    if (!com->send(&out, k.data(), ar.data())) {
      qDebug() << "IPlot server: Failed to send response";
      QApplication::quit();
    }
  } break;
  case QVariant::ULongLong: {
    uint64_t v = val.toULongLong();
    out.argtype[1] = Responder::DT_UInt64;
    out.arglen[1] = sizeof(uint64_t);
    if (!com->send(&out, k.data(), &v)) {
      qDebug() << "IPlot server: Failed to send response";
      QApplication::quit();
    }
  } break;
  case QVariant::Int: {
    int32_t v = val.toInt();
    out.argtype[1] = Responder::DT_Int32;
    out.arglen[1] = sizeof(int32_t);
    if (!com->send(&out, k.data(), &v)) {
      qDebug() << "IPlot server: Failed to send response";
      QApplication::quit();
    }
  } break;
  case QVariant::Double: {
    double v = val.toDouble();
    out.argtype[1] = Responder::DT_Double;
    out.arglen[1] = sizeof(double);
    if (!com->send(&out, k.data(), &v)) {
      qDebug() << "IPlot server: Failed to send response";
      QApplication::quit();
    }
  } break;
  case QVariant::PointF: case QVariant::Point: {
    QPointF v0 = val.toPointF();
    IVector v; v << v0.x() << v0.y();
    out.argtype[1] = Responder::DT_Double;
    out.arglen[1] = sizeof(double)*2;
    if (!com->send(&out, k.data(), v.data())) {
      qDebug() << "IPlot server: Failed to send response";
      QApplication::quit();
    }
  } break;
  case QVariant::SizeF: case QVariant::Size: {
    QSizeF v0 = val.toSizeF();
    IVector v; v << v0.width() << v0.height();
    out.argtype[1] = Responder::DT_Double;
    out.arglen[1] = sizeof(double)*2;
    if (!com->send(&out, k.data(), v.data())) {
      qDebug() << "IPlot server: Failed to send response";
      QApplication::quit();
    }
  } break;
  case QMetaType::QColor: {
    QColor c = val.value<QColor>();
    IVector v;
    v << c.red()/255.0 << c.green()/255.0 << c.blue()/255.0;
    out.argtype[1] = Responder::DT_Double;
    out.arglen[1] = sizeof(double)*3;
    if (!com->send(&out, k.data(), v.data())) {
      qDebug() << "IPlot server: Failed to send response";
      QApplication::quit();
    }
  } break;
  case QVariant::UserType: {
    // convert to IVector or IHandleSet, even if an empty one
    IHandleSet hs = val.value<IHandleSet>();
    if (hs.isEmpty()) {
      IVector v = val.value<IVector>();
      out.argtype[1] = Responder::DT_Double;
      out.arglen[1] = v.size()*sizeof(double);
      if (!com->send(&out, k.data(), v.data())) {
        qDebug() << "IPlot server: Failed to send response";
        QApplication::quit();
      }
    } else {
      QVector<uint64_t> v;
      for (auto h: hs)
        v << h;
      out.argtype[1] = Responder::DT_UInt64;
      out.arglen[1] = v.size()*sizeof(uint64_t);
      if (!com->send(&out, k.data(), v.data())) {
        qDebug() << "IPlot server: Failed to send response";
        QApplication::quit();
      }
    }
  } break;
  default: {
    bool ok;
    double v = val.toDouble(&ok);
    if (ok) {
      out.argtype[1] = Responder::DT_Double;
      out.arglen[1] = sizeof(double);
      if (!com->send(&out, k.data(), &v)) {
        qDebug() << "IPlot server: Failed to send response";
        QApplication::quit();
      }
    } else {
      sendResponse(Command::BadValue);
    }
  } break;
  }
}

void IResponder::callback(IHandle h, QString key, QVariant arg) {
  Responder::Header out;
  Responder::prepareHeader(&out);
  out.seqid = 1;
  out.handle = h;
  out.cmd = Command::Callback;
  sendCallbackOrValue(out, key, arg);
}
