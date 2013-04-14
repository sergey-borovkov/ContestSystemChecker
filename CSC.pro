TEMPLATE = app
TARGET = csc
CONFIG += console

# Input
HEADERS += \
    checker.h \
    compiler.h \
    submissionprocess.h \
    debugger.h \
    util.h
SOURCES += \
    checker.cpp \
    compiler.cpp \
    main.cpp \
    submissionprocess.cpp \
    debugger.cpp \
    util.cpp
