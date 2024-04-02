#pragma once

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#define POSITION position
#define GLOBAL_POSITION globalPosition
#define POINTER_TYPE QPointingDevice::PointerType

#else

#define POSITION posF
#define GLOBAL_POSITION globalPosF
#define POINTER_TYPE QTabletEvent::PointerType

#endif

