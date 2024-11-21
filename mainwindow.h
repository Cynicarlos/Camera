#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QFileDialog>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QMessageBox>
#include "videocapturefilter.h"
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
    void init();
    void init_Camera();
    void updateFrame();            // 定时更新视频帧
    void on_Devices_Selected(int selected_deviceId, QAction *selectedAction);
    //菜单栏槽函数
    void on_Action_File_Set_Capture_Directory();
    void on_Action_File_Set_Photo_Directory();
    //void on_Action_File_Save_Captured_Video_As();
    void on_Action_File_Exit();

    void on_Action_Option_Video_Capture_Filter();
    void on_Action_Option_Video_Capture_pin();

    void on_Action_Capture_Capture_Image();
    void on_Action_Capture_Capture_Audio();
    void on_Action_Capture_Capture_Buffer_Data();
    void on_Action_Capture_Capture_Prevciew_Picture();
    void on_Action_Capture_Set_Frame_Rate();
    void on_Action_Capture_Set_Time_Limit();
    void on_Action_Capture_Set_Video_Compressor();
    //void on_Action_Capture_Start_Capturen();
    //void on_Action_Capture_Stop_Capture_ESC();
    void on_Action_Capture_Master_Stream_None();
    void on_Action_Capture_Master_Stream_Video();
    void on_Action_Capture_Master_Stream_Audio();
    void on_Action_Photo_Start_Photo_F3();
    void on_Action_Photo_Set_Photo_Format();

    void on_Action_Help_About();

    void startVideoRecording();
    void stopVideoRecording();


private:
    Ui::MainWindow *ui;
    QMenuBar *menuBar;
    QLabel *videoLabel;
    cv::VideoCapture cap;          // OpenCV 摄像头捕获对象
    QTimer *timer;                 // 定时器，用于刷新视频帧
    int device_id = 0;

    //video recorder
    cv::VideoWriter writer;  // 用于视频录制
    bool isRecording = false; // 标记是否正在录制
    int frameWidth = 640;    // 视频帧宽度
    int frameHeight = 480;   // 视频帧高度
    int fps = 30;            // 视频帧率
    QTimer *recordTimer;     // 定时器，用于定时捕获视频帧

    QString video_saved_path;
    QString image_saved_path;

    QMenu *menu_devices;

    //Files
    QAction *action_File_Set_Capture_Directory;
    QAction *action_File_Set_Photo_Directory;
    QAction *action_File_Save_Captured_Video_As;
    QAction *action_File_Exit;

    //Options
    QAction *action_Option_Preview;
    QAction *action_Option_Video_Capture_Filter;
    QAction *action_Option_Video_Capture_pin;

    //Capture
    QAction *action_Capture_Capture_Image;
    QAction *action_Capture_Capture_Audio;
    QAction *action_Capture_Capture_Buffer_Data;
    QAction *action_Capture_Capture_Prevciew_Picture;
    QAction *action_Capture_Set_Frame_Rate;
    QAction *action_Capture_Set_Time_Limit;
    QAction *action_Capture_Set_Video_Compressor;
    QAction *action_Capture_Start_Capture;
    QAction *action_Capture_Stop_Capture;

    QAction *action_Capture_Master_Stream_None;
    QAction *action_Capture_Master_Stream_Video;
    QAction *action_Capture_Master_Stream_Audio;

    //Photo
    QAction *action_Photo_Start_Photo_F3;
    QAction *action_Photo_Set_Photo_Format;

    //Help
    QAction *action_Help_About;



    // int brightness;                // 当前亮度值
    // int contrast;                  // 当前对比度值
};

#endif // MAINWINDOW_H
