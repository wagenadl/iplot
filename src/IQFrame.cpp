// IQFrame.cpp

#include "IQFrame.h"
#include <QGridLayout>
#include "IQXAxis.h"
#include "IQYAxis.h"
#include "IQAxes.h"
#include <QPainter>

IQFrame::IQFrame(IAxes *parent) {
  QGridLayout *layout = new QGridLayout();
  w = new IQAxes(parent);
  w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  layout->addWidget(w, 0, 1);
  xaxis = new IQXAxis();
  yaxis = new IQYAxis();
  layout->addWidget(xaxis, 1, 1);
  layout->addWidget(yaxis, 0, 0);

  QPalette p(palette());
  p.setColor(QPalette::Window, QColor("#ffffff"));
  setPalette(p);
  setFrameShadow(QFrame::Sunken);
  setFrameShape(QFrame::Panel);
  setAutoFillBackground(true);
  setLayout(layout);
}

void IQFrame::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.save();
  p.translate(w->pos());
  w->paint(p);
  p.restore();
  p.save();
  p.translate(xaxis->pos());
  xaxis->paint(p);
  p.restore();
  p.save();
  p.translate(yaxis->pos());
  yaxis->paint(p);
  p.restore();
}
