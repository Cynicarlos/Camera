#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QFileDialog>
#include "VideoCaptureFilter.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateFrame();            // 定时更新视频帧
    // void updateBrightness(int);    // 更新亮度
    // void updateContrast(int);      // 更新对比度

    //菜单栏槽函数
    void on_Action_File_Set_Capture_Directory();
    void on_Action_File_Set_Photo_Directory();
    void on_Action_File_Save_Captured_Video_As();
    void on_Action_File_Exit();

    void on_Action_Option_Preview();
    void on_Action_Option_Video_Capture_Filter();
    void on_Action_Option_Video_Capture_pin();

    void on_Action_Capture_Capture_Audio();
    void on_Action_Capture_Capture_Buffer_Data();
    void on_Action_Capture_Capture_Prevciew_Picture();
    void on_Action_Capture_Set_Frame_Rate();
    void on_Action_Capture_Set_Time_Limit();
    void on_Action_Capture_Set_Video_Compressor();
    void on_Action_Capture_Start_Capturen();
    void on_Action_Capture_Stop_Capture_ESC();
    void on_Action_Capture_Master_Stream_None();
    void on_Action_Capture_Master_Stream_Video();
    void on_Action_Capture_Master_Stream_Audio();
    void on_Action_Photo_Start_Photo_F3();
    void on_Action_Photo_Set_Photo_Format();

    void on_Action_Help_About();



private:
    Ui::MainWindow *ui;
    QMenuBar *menuBar;
    QLabel *videoLabel;
    cv::VideoCapture cap;          // OpenCV 摄像头捕获对象
    QTimer *timer;                 // 定时器，用于刷新视频帧



    // int brightness;                // 当前亮度值
    // int contrast;                  // 当前对比度值
};

#endif // MAINWINDOW_H
