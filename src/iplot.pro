# QMake project file for iplot                -*- mode: shell-script; -*-
  
TEMPLATE = app
TARGET = ../iplot
CONFIG += debug_and_release
QMAKE_CXXFLAGS += -std=c++11
OBJECTS_DIR=../release

CONFIG(debug, debug|release) { TARGET=$${TARGET}_debug }
CONFIG(debug, debug|release) { OBJECTS_DIR=../debug }

MOC_DIR = $${OBJECTS_DIR}
RCC_DIR = $${OBJECTS_DIR}
UI_DIR = $${OBJECTS_DIR}

HEADERS += IObject.h IFigure.h IQFigure.h IAxes.h IQAxes.h \
    IItem.h ILine.h IPoints.h IPatch.h IText.h IButton.h \
    LRBase.h Responder.h IResponder.h CallbackHandler.h IVector.h IHandle.h \
    IQBaseAxis.h IQXAxis.h IQYAxis.h IQFrame.h


SOURCES += IObject.cpp IFigure.cpp IQFigure.cpp IAxes.cpp IQAxes.cpp \
    IItem.cpp ILine.cpp IPoints.cpp IPatch.cpp IText.cpp IButton.cpp \
    LRBase.cpp Responder.cpp IResponder.cpp CallbackHandler.cpp \
    IQBaseAxis.cpp IQXAxis.cpp IQYAxis.cpp IQFrame.cpp \
    main.cpp
