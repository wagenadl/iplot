// IText.cpp

#include "IText.h"
#include "IAxes.h"
#include <QPainter>
#include <QDebug>

IText::IText(class IAxes *parent): IItem(parent), parent(parent) {
  props["type"] = "text";
  props["pos"] = QPointF(0, 0);
  props["text"] = "";
  props["halign"] = "left";
  props["valign"] = "middle";
  props["color"] = QColor(0, 0, 0);
}

IText::IText(class IAxes *parent, QPointF pos, QString txt): IText(parent) {
  props["pos"] = pos;
  props["text"] = txt;
  parent->update();
}

bool IText::set(QString key, QVariant value) {
  if (key=="halign") {
    QString v = value.toString();
    if (!(v=="left" || v=="center" || v=="right"))
      return false;
  } else if (key=="valign") {
    QString v = value.toString();
    if (!(v=="top" || v=="middle" || v=="bottom"))
      return false;
  }  
  bool r = IItem::set(key, value);
  parent->update();
  return r;
}

QSet<QString> const &IText::properties() const {
  static QSet<QString> ss;
  if (ss.isEmpty()) {
    ss = IItem::properties();
    ss << "text";
    ss << "pos";
    ss << "halign";
    ss << "valign";
  }
  return ss;
}

void IText::paint(class QPainter &p) {
  p.setPen(QPen(color()));
  QPointF pos = parent->mapFromData(getAsPointF("pos"));
  QString txt = get("text").toString();
  QString ha = get("halign").toString();
  QString va = get("valign").toString();
  double l = pos.x();
  double r = pos.x();
  double t = pos.y();
  double b = pos.y();
  if (ha=="right" || ha=="center") 
    l -= 1000;
  if (ha=="left" || ha=="center")
    r += 1000;
  if (va=="bottom" || va=="middle")
    t -= 1000;
  if (va=="top" || va=="middle")
    b += 1000;

  p.drawText(QRectF(QPointF(l,t), QPointF(r,b)),
             (ha=="left" ? Qt::AlignLeft
              : ha=="right" ? Qt::AlignRight
              : Qt::AlignHCenter)
             | (va=="top" ? Qt::AlignTop
                : va=="bottom" ? Qt::AlignBottom
                : Qt::AlignVCenter),
             txt);
}
