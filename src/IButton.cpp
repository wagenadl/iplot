// IButton.cpp

#include "IButton.h"
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include "IFigure.h"

IButton::IButton(IFigure *parent, QString typ, QString txt):
  IObject(parent) {
  Q_ASSERT(types().contains(typ));
  props["type"] = "button";
  props["text"] = txt;

  if (typ=="push")
    w = new QPushButton(txt);
  else if (typ=="check")
    w = new QCheckBox(txt);
  else if (typ=="label")
    w = new QLabel(txt);
  else
    Q_ASSERT(0);

  w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

  if (typ=="push" || typ=="check")
    connect(w, SIGNAL(clicked(bool)), SLOT(activate()));
}

IButton::~IButton() {
  delete w;
}

void IButton::activate() {
  QWidget *ww = w;
  QAbstractButton *b = dynamic_cast<QAbstractButton *>(ww);
 //  qDebug() << "activate " << b;
  if (b)
    call("buttondownfcn", b->isChecked());
}

QSet<QString> const &IButton::types() {
  static QSet<QString> tt;
  if (tt.isEmpty()) {
    tt << "push";
    tt << "check";
    tt << "label";
  }
  return tt;
}

QSet<QString> const &IButton::properties() const {
  static QSet<QString> ss;
  if (ss.isEmpty()) {
    ss = IObject::properties();
    ss << "text";
    ss << "buttondownfcn";
    ss << "checked";
  }
  return ss;
}

bool IButton::set(QString key, QVariant value) {
  bool res = IObject::set(key, value);
  if (key=="text") {
    QWidget *ww = w;
    QLabel *l = dynamic_cast<QLabel*>(ww);
    if (l)
      l->setText(value.toString());
    QAbstractButton *b = dynamic_cast<QAbstractButton*>(ww);
    if (b)
      b->setText(value.toString());
  } else if (key=="checked") {
    QWidget *ww = w;
    QAbstractButton *b = dynamic_cast<QAbstractButton*>(ww);
    if (b)
      b->setChecked(getAsDouble("checked")>0);
    else
      return false;
  }
  return res;
}

QVariant IButton::get(QString key) const {
  if (key=="checked") {
    QWidget *ww = w;
    QAbstractButton *b = dynamic_cast<QAbstractButton*>(ww);
    if (b) 
      return QVariant(b->isChecked() ? 1.0 : 0.0);
    else
      return QVariant();
  } else {
    return IObject::get(key);
  }
}
