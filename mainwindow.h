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
#include <QSettings>
#include <QDate>
#include <QWidget>
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
    void checkTrialPeriod();
    void init();
    void init_Camera();
    void updateFrame();            // 定时更新视频帧
    void on_Devices_Selected(int selected_deviceId, QAction *selectedAction);
    //菜单栏槽函数
    void on_Action_File_Set_Capture_Directory();
    void on_Action_File_Set_Photo_Directory();
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
    void on_Action_Capture_Master_Stream_None();
    void on_Action_Capture_Master_Stream_Video();
    void on_Action_Capture_Master_Stream_Audio();
    void on_Action_Photo_Start_Photo_F3();
    void on_Action_Photo_Set_Photo_Format();

    void on_Action_Help_About();

    void startVideoRecording();
    void stopVideoRecording();

    //utils
    //QSize get_Max_Resolution(QList<QSize>);


private:
    bool is_owner = true; //开发时为true,发布时改为false
    Ui::MainWindow *ui;
    QMenuBar *menuBar;
    QMenu *menu_devices;
    VideoCaptureFilter *video_capture_filter;
    QLabel *videoLabel;
    cv::VideoCapture cap;          // OpenCV 摄像头捕获对象
    QTimer *timer;                 // 定时器，用于刷新视频帧
    std::unique_ptr<QList<QCameraDevice>> cameras; //可用的摄像头列表
    int device_id = 0;


    //video parameter
    int brightness;
    double contrast;
    int hue;
    int saturability;
    int plain;
    double gamma;
    int wb;
    bool WB_ABILITY = true;
    bool autoWB = false;
    int backlight;
    int gain;

    // int brightness = 0;
    // double contrast  = 32.0;
    // int hue = 0;
    // int saturability =64;
    // int plain = 1;
    // double gamma = 2.2;
    // int wb = 5500;
    // bool WB_ABILITY = true;
    // bool autoWB = false;
    // int backlight = 1;
    // int gain;

    //video recorder
    cv::VideoWriter writer;  // 用于视频录制
    bool isRecording = false; // 标记是否正在录制
    int fps = 30;            // 视频帧率
    QTimer *recordTimer;     // 定时器，用于定时捕获视频帧

    QString video_saved_path;
    QString image_saved_path;

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
    QAction *action_Capture_Start_Recording;
    QAction *action_Capture_Stop_Recording;

    QAction *action_Capture_Capture_Audio;
    QAction *action_Capture_Capture_Buffer_Data;
    QAction *action_Capture_Capture_Prevciew_Picture;
    QAction *action_Capture_Set_Frame_Rate;
    QAction *action_Capture_Set_Time_Limit;
    QAction *action_Capture_Set_Video_Compressor;

    QAction *action_Capture_Master_Stream_None;
    QAction *action_Capture_Master_Stream_Video;
    QAction *action_Capture_Master_Stream_Audio;

    //Photo
    QAction *action_Photo_Start_Photo_F3;
    QAction *action_Photo_Set_Photo_Format;

    //Help
    QAction *action_Help_About;

    void Get_Amp_Default_Values();

    void onVideoCaptureFilterBrightnessChanged(int value); // 处理子窗口信号的槽函数
    void onVideoCaptureFilterContrastChanged(int value);
    void onVideoCaptureFilterHueChanged(int value);
    void onVideoCaptureFilterSaturabilityChanged(int value);
    void onVideoCaptureFilterPlainChanged(int value);
    void onVideoCaptureFilterGammaChanged(int value);
    void onVideoCaptureFilterWbChanged(int value);
    void onVideoCaptureFilterBacklightChanged(int value);
    void onVideoCaptureFilterGainChanged(int value);

    void onVideoCaptureFilterAutoWBChanged(bool value);

    void onVideoCaptureFilterAmpYesClicked(QString h, QString w, QString denoise_level);

};

#endif // MAINWINDOW_H

