// IPatch.h

#ifndef IPATCH_H

#define IPATCH_H

#include "ILine.h"
#include <QPolygonF>

class IPatch: public ILine {
public:
  IPatch(IAxes *parent);
  IPatch(IAxes *parent, class IVector const &xx, class IVector const &yy);
  virtual ~IPatch();
  virtual QSet<QString> const &properties() const override;
  static IPatch *find(qulonglong handle) {
    return dynamic_cast<IPatch*>(IObject::find(handle)); }
  virtual void paint(class QPainter &) override;
  virtual bool hitTest(QPointF) const override;
};

#endif
