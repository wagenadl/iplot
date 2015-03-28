// IVector.h

#ifndef IVECTOR_H

#define IVECTOR_H

#include <QVector>
#include <QMetaType>

class IVector: public QVector<double> {
};
Q_DECLARE_METATYPE(IVector);

#endif
