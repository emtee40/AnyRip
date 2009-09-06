SOURCES += main.cpp \
    dvdimagejob.cpp \
    imagegui.cpp \
    job.cpp \
    video.cpp \
    dvddrive.cpp
HEADERS += dvdimagejob.h \
    imagegui.h \
    job.h \
    video.h \
    dvddrive.h
LIBS += -ldvdcss \
    -ldvdread
QT += dbus
