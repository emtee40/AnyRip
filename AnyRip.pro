SOURCES += main.cpp \
    dvdimagejob.cpp \
    dvdimagejobgui.cpp \
    job.cpp \
    video.cpp \
    dvddrive.cpp \
    jobqueue.cpp \
    videoqueue.cpp \
    encodemp4job.cpp \
    uploadjob.cpp \
    titleloadjob.cpp \
    videogui.cpp \
    mainwindow.cpp
HEADERS += dvdimagejob.h \
    dvdimagejobgui.h \
    job.h \
    video.h \
    dvddrive.h \
    jobqueue.h \
    videoqueue.h \
    encodemp4job.h \
    uploadjob.h \
    titleloadjob.h \
    videogui.h \
    mainwindow.h
LIBS += -ldvdcss \
    -ldvdread
QT += dbus
