QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    videoProcessing.cpp \
    videocapturefilter.cpp

HEADERS += \
    mainwindow.h \
    videoProcessing.h \
    videocapturefilter.h

FORMS += \
    mainwindow.ui \
    videocapturefilter.ui

INCLUDEPATH += D:\Programing\opencv\OpenCV-MinGW-Build-OpenCV-4.5.5-x64\include\
               D:\Programing\opencv\OpenCV-MinGW-Build-OpenCV-4.5.5-x64\include\opencv2\
               D:\Programing\opencv\OpenCV-MinGW-Build-OpenCV-4.5.5-x64\include\opencv
LIBS += -L D:\Programing\opencv\OpenCV-MinGW-Build-OpenCV-4.5.5-x64\x64\mingw\lib\libopencv_*.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target





