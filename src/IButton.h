// IButton.h

#ifndef IBUTTON_H

#define IBUTTON_H

#include "IObject.h"
#include <QPointer>
#include <QWidget>

class IButton: public IObject {
  Q_OBJECT;
public:
  IButton(class IFigure *parent, QString typ="push", QString txt="");
  virtual ~IButton();
  virtual QSet<QString> const &properties() const override;
  virtual bool set(QString key, QVariant value) override;
  virtual QVariant get(QString key) const override;
  QWidget *widget() const { return w; }
  static QSet<QString> const &types();
private slots:
  void activate();
private:
  QPointer<QWidget> w;
};

#endif
