// IAxes.h

#ifndef IAXES_H

#define IAXES_H

#include "IObject.h"
#include <QPolygonF>
#include "IQFrame.h"
#include <QPointer>

class IAxes: public IObject {
public:
  IAxes(class IFigure *parent);
  virtual ~IAxes();
  virtual bool set(QString key, QVariant value);
  virtual QSet<QString> const &properties() const;
  static IAxes *find(qulonglong handle) {
    return dynamic_cast<IAxes*>(IObject::find(handle)); }
  QWidget *widget() { return frame; }
  QPointF mapToData(QPointF widgetcoords) const; // from pixels
  QPointF mapFromData(QPointF datacoords) const; // to pixels
  QPolygonF mapFromData(class IVector const &xx, class IVector const &yy) const;
  void mouseEvent(QString type, QPointF widgetcoords, int button);
  void update();
  void updateLimits();
  void paint(class QPainter &);
  static IAxes *ensureGCA();
private:
  QPointer<IQFrame> frame;
};

#endif
