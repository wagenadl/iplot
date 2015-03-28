// IFigure.h

#ifndef IFIGURE_H

#define IFIGURE_H

#include "IObject.h"
#include <QHash>

class IFigure: public IObject {
public:
  IFigure(IObject *parent=0);
  IFigure(QSize size, IObject *parent=0);
  IFigure(QRect geom, IObject *parent=0);
  virtual ~IFigure();
  virtual bool set(QString key, QVariant value);
  virtual QSet<QString> const &properties() const;
  static IFigure *find(IHandle handle) {
    return dynamic_cast<IFigure*>(IObject::find(handle)); }
  static IFigure *ensureGCF();
  class IAxes *subplot(int rows, int columns, int row, int col);
  class IAxes *subplot(int row, int col);
  class IAxes *subplot(int rows, int columns, int n);
  class IAxes *subplot(int n);
  IHandle currentAxes();
  void selectAxes(IHandle);
  class IQFigure *widget() { return w; }
  class IButton *newButton(QString typ, QString txt);
  void updateGeometry();
  static IHandleSet all();
protected:
  class IQFigure *w;
  int nrows, ncolumns;
  IHandle gca_;
  QHash<QPoint, IHandle> axes;
};

#endif
