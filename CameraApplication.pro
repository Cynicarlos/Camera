QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    videocapturefilter.cpp

HEADERS += \
    mainwindow.h \
    videocapturefilter.h

FORMS += \
    mainwindow.ui \
    videocapturefilter.ui

INCLUDEPATH += D:\Programing\opencv\OpenCV-MinGW-Build-OpenCV-4.5.5-x64\include\
               D:\Programing\opencv\OpenCV-MinGW-Build-OpenCV-4.5.5-x64\include\opencv2\
               D:\Programing\opencv\OpenCV-MinGW-Build-OpenCV-4.5.5-x64\include\opencv
LIBS += -L D:\Programing\opencv\OpenCV-MinGW-Build-OpenCV-4.5.5-x64\x64\mingw\lib\libopencv_*.a

# win32:CONFIG(release, debug|release): LIBS += -LD:/Programing/opencv/build/x64/vc14/lib/ -lopencv_world455
# else:win32:CONFIG(debug, debug|release): LIBS += -LD:/Programing/opencv/build/x64/vc14/lib/ -lopencv_world455d
# else:unix: LIBS += -LD:/Programing/opencv/build/x64/vc14/lib/ -lopencv_world455

# win32:CONFIG(release, debug|release): LIBS += -LD:/Programing/opencv/build/x64/vc15/lib/ -lopencv_world455
# else:win32:CONFIG(debug, debug|release): LIBS += -LD:/Programing/opencv/build/x64/vc15/lib/ -lopencv_world455d
# else:unix: LIBS += -LD:/Programing/opencv/build/x64/vc15/lib/ -lopencv_world455

# INCLUDEPATH += D:/Programing/opencv/build/include
# DEPENDPATH += D:/Programing/opencv/build/include


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target





