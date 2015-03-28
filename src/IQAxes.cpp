// IQAxes.cpp

#include "IQAxes.h"
#include <QMouseEvent>
#include "IAxes.h"
#include <QPalette>
#include <QPainter>

IQAxes::IQAxes(class IAxes *parent): QWidget(), parent(parent) {
}

IQAxes::~IQAxes() {
}

void IQAxes::mousePressEvent(QMouseEvent *e) {
  parent->mouseEvent("down", e->pos(), e->button());
}

void IQAxes::mouseMoveEvent(QMouseEvent *e) {
  parent->mouseEvent("motion", e->pos(), e->button());
}

void IQAxes::mouseReleaseEvent(QMouseEvent *e) {
  parent->mouseEvent("up", e->pos(), e->button());
};

void IQAxes::paint(QPainter &p) {
  parent->paint(p);
}
