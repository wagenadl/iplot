// IHandle.h

#ifndef IHANDLE_H

#define IHANDLE_H

#include <QVariant>
#include <QSet>

typedef qulonglong IHandle;

class IHandleSet: public QSet<IHandle> {
};
Q_DECLARE_METATYPE(IHandle);
Q_DECLARE_METATYPE(IHandleSet);

#endif
