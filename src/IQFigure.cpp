// IQFigure.cpp

#include "IQFigure.h"
#include <QMouseEvent>
#include "IFigure.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

IQFigure::IQFigure(class IFigure *parent): QWidget(), parent(parent) {
  blay = new QHBoxLayout();
  blay->addStretch(1);
  axlay = new QGridLayout();
  lay = new QVBoxLayout();
  lay->addLayout(blay);
  lay->addLayout(axlay);
  setLayout(lay);
}

IQFigure::~IQFigure() {
}

void IQFigure::mousePressEvent(QMouseEvent *e) {
  parent->set("currentpoint", e->pos());
  parent->call("windowbuttondownfcn", e->button());
};

void IQFigure::mouseMoveEvent(QMouseEvent *e) {
  parent->set("currentpoint", e->pos());
  parent->call("windowbuttonmotionfcn", e->button());
};

void IQFigure::mouseReleaseEvent(QMouseEvent *e) {
  parent->set("currentpoint", e->pos());
  parent->call("windowbuttonupfcn", e->button());
};

void IQFigure::moveEvent(QMoveEvent *) {
  parent->updateGeometry();
}

void IQFigure::resizeEvent(QResizeEvent *) {
  parent->updateGeometry();
}

void IQFigure::closeEvent(QCloseEvent *) {
  parent->deleteLater();
}
