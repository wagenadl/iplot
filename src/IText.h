// IText.h

#ifndef ITEXT_H

#define ITEXT_H

#include "IItem.h"

class IText: public IItem {
public:
  IText(class IAxes *parent);
  IText(class IAxes *parent, QPointF pos, QString txt);
  virtual ~IText() { }
  virtual bool set(QString key, QVariant value) override;
  virtual QSet<QString> const &properties() const override;
  static IText *find(qulonglong handle) {
    return dynamic_cast<IText*>(IObject::find(handle)); }
  virtual void paint(class QPainter &) override;
protected:
  class IAxes *parent;
};

#endif
