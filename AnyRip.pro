SOURCES += main.cpp \
	dvdimagejob.cpp \
    imagegui.cpp \
    job.cpp \
    video.cpp
HEADERS += dvdimagejob.h \
    imagegui.h \
    job.h \
    video.h
LIBS += -ldvdcss \
    -ldvdread
