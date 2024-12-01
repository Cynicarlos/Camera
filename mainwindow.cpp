#include "mainwindow.h"
#include "videoProcessing.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setStyleSheet("QMenu::item:selected {background-color:rgb(234, 243, 253);}");
    // 检查试用期是否结束
    if(!is_owner){
        checkTrialPeriod();
    }
    init();
    init_Camera();
}

MainWindow::~MainWindow()
{
    cap.release(); // 释放摄像头资源lk.
    delete ui;
}

void MainWindow::checkTrialPeriod()
{
    QSettings settings("IAT", "Camera");
    bool isFirstRun = settings.value("isFirstRun", true).toBool();
    if (isFirstRun) {
        QDate date = QDate::currentDate();
        settings.setValue("firstRunDate", date.toString(Qt::ISODate));
        qDebug() << date.year() << date.month() << date.day();
        settings.setValue("isFirstRun", false);
    }

    QString firstRunDateStr = settings.value("firstRunDate").toString();
    QDate firstRunDate = QDate::fromString(firstRunDateStr, Qt::ISODate);

    QDate currentDate = QDate::currentDate();
    qint64 daysDifference = firstRunDate.daysTo(currentDate);
    qDebug() << daysDifference;
    if (daysDifference > 1) { // 假设试用期为1天
        QMessageBox::warning(this, "试用期结束", "您的试用期已结束，如有需要请联系开发者。");

        // 程序继续运行时可以主动退出
        QTimer::singleShot(0, &QApplication::quit); // 使用 QTimer::singleShot 延迟退出
    }
}


void MainWindow::init(){
    // 创建菜单栏
    menuBar = ui->menubar;
    videoLabel = ui->videoLabel;
    videoLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    videoLabel->setScaledContents(true); // 保持图片比例
    videoLabel->setAlignment(Qt::AlignCenter); // 居中显示videoLabel = ui->videoLabel;

    //Files
    action_File_Set_Capture_Directory = ui->action_File_Set_Video_Directory;
    action_File_Set_Photo_Directory = ui->action_File_Set_Image_Directory;
    action_File_Save_Captured_Video_As = ui->action_File_Save_Captured_Video_As;
    action_File_Exit = ui->action_File_Exit;

    //Options
    action_Option_Video_Capture_Filter = ui->action_Option_Video_Capture_Filter;
    action_Option_Video_Capture_pin = ui->action_Option_Video_Capture_pin;

    //Capture
    action_Capture_Capture_Image = ui->action_Capture_Capture_Image;
    action_Capture_Capture_Image->setShortcut(QKeySequence(Qt::Key_F2));

    action_Capture_Capture_Audio = ui->action_Capture_Capture_Audio;
    action_Capture_Capture_Buffer_Data = ui->action_Capture_Capture_Buffer_Data;
    action_Capture_Capture_Prevciew_Picture = ui->action_Capture_Capture_Prevciew_Picture;
    action_Capture_Set_Frame_Rate = ui->action_Capture_Set_Frame_Rate;
    action_Capture_Set_Time_Limit = ui->action_Capture_Set_Time_Limit;
    action_Capture_Set_Video_Compressor = ui->action_Capture_Set_Video_Compressor;

    action_Capture_Start_Recording = ui->action_Capture_Start_Recording;
    action_Capture_Start_Recording->setShortcut(QKeySequence(Qt::Key_F3));

    action_Capture_Stop_Recording = ui->action_Capture_Stop_Recording;
    action_Capture_Stop_Recording->setShortcut(QKeySequence(Qt::Key_Escape));

    action_Capture_Master_Stream_None = ui->action_Capture_Master_Stream_None;
    action_Capture_Master_Stream_Video = ui->action_Capture_Master_Stream_Video;
    action_Capture_Master_Stream_Audio = ui->action_Capture_Master_Stream_Audio;

    //Photo
    action_Photo_Start_Photo_F3 = ui->action_Photo_Start_Photo_F3;
    action_Photo_Set_Photo_Format = ui->action_Photo_Set_Photo_Format;

    //Help
    action_Help_About = ui->action_Help_About;


    connect(action_File_Set_Capture_Directory, &QAction::triggered, this, &MainWindow::on_Action_File_Set_Capture_Directory);
    connect(action_File_Set_Photo_Directory, &QAction::triggered, this, &MainWindow::on_Action_File_Set_Photo_Directory);
    connect(action_File_Exit, &QAction::triggered, this, &MainWindow::on_Action_File_Exit);

    connect(action_Option_Video_Capture_Filter, &QAction::triggered, this, &MainWindow::on_Action_Option_Video_Capture_Filter);
    connect(action_Option_Video_Capture_pin, &QAction::triggered, this, &MainWindow::on_Action_Option_Video_Capture_pin);

    connect(action_Capture_Capture_Image, &QAction::triggered, this, &MainWindow::on_Action_Capture_Capture_Image);
    connect(action_Capture_Capture_Audio, &QAction::triggered, this, &MainWindow::on_Action_Capture_Capture_Audio);
    connect(action_Capture_Capture_Buffer_Data, &QAction::triggered, this, &MainWindow::on_Action_Capture_Capture_Buffer_Data);
    connect(action_Capture_Capture_Prevciew_Picture, &QAction::triggered, this, &MainWindow::on_Action_Capture_Capture_Prevciew_Picture);
    connect(action_Capture_Set_Frame_Rate, &QAction::triggered, this, &MainWindow::on_Action_Capture_Set_Frame_Rate);
    connect(action_Capture_Set_Time_Limit, &QAction::triggered, this, &MainWindow::on_Action_Capture_Set_Time_Limit);
    connect(action_Capture_Set_Video_Compressor, &QAction::triggered, this, &MainWindow::on_Action_Capture_Set_Video_Compressor);
    connect(action_Capture_Start_Recording, &QAction::triggered, this, &MainWindow::startVideoRecording);
    connect(action_Capture_Stop_Recording, &QAction::triggered, this, &MainWindow::stopVideoRecording);
    connect(action_Capture_Master_Stream_None, &QAction::triggered, this, &MainWindow::on_Action_Capture_Master_Stream_None);
    connect(action_Capture_Master_Stream_Video, &QAction::triggered, this, &MainWindow::on_Action_Capture_Master_Stream_Video);
    connect(action_Capture_Master_Stream_Audio, &QAction::triggered, this, &MainWindow::on_Action_Capture_Master_Stream_Audio);

    connect(action_Photo_Start_Photo_F3, &QAction::triggered, this, &MainWindow::on_Action_Photo_Start_Photo_F3);
    connect(action_Photo_Set_Photo_Format, &QAction::triggered, this, &MainWindow::on_Action_Photo_Set_Photo_Format);

    connect(action_Help_About, &QAction::triggered, this, &MainWindow::on_Action_Help_About);

}


void MainWindow::init_Camera() {
    menu_devices = ui->menuDevices;
    cameras = std::make_unique<QList<QCameraDevice>>(QMediaDevices::videoInputs());
    //QList<QSize> resolutions; //当前摄像头支持的分辨率

    // 遍历所有视频输入设备
    for (int i = 0; i < cameras->size(); ++i) {
        QCameraDevice cameraDevice = cameras->at(i);
        QString deviceDescription = cameraDevice.description();

        // 创建一个可勾选的 QAction
        QAction *cameraAction = new QAction(deviceDescription, menu_devices);
        cameraAction->setCheckable(true);  // 设置为可勾选
        //resolutions = cameraDevice.photoResolutions();//当前相机支持的分辨率
        if(i == 0){
            cameraAction->setChecked(true);//给默认设备打勾
        }
        menu_devices->addAction(cameraAction);

        // 连接信号和槽
        connect(cameraAction, &QAction::triggered, this, [this, i, cameraAction]() {
            on_Devices_Selected(i, cameraAction);  // 传递设备ID和对应的 QAction
        });
    }

    cap.open(device_id);
    if (!cap.isOpened()) {
        qFatal("Failed to open camera");
        return;
    }
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(30);  // 每 30 毫秒更新一次（约 33 FPS）
}


void MainWindow::on_Devices_Selected(int selected_deviceId, QAction *selectedAction) {
    if(device_id == selected_deviceId) return;
    cap.open(selected_deviceId);
    if (!cap.isOpened()) {
        qFatal("Failed to open camera");
        return;
    }
    device_id = selected_deviceId;

    // 取消所有其他 QAction 的勾选状态
    for (QAction *action : menu_devices->actions()) {
        action->setChecked(false);
    }
    // 勾选当前选中的 QAction
    selectedAction->setChecked(true);

    // 启动定时器
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(30);  // 每 30 毫秒更新一次（约 33 FPS）
}

void MainWindow::updateFrame()
{
    cv::Mat frame;
    cap >> frame; // 从摄像头获取一帧
    if (frame.empty())
        return;
    // frame = adjustBrightness(frame,1.0,brightness); //调整亮度
    // frame = adjustHue(frame,hue);
    // // frame = adjustSaturation(frame,saturability);
    if(autoWB)
    {
        frame = grayWorld(frame);
    }
    if (!WB_ABILITY && !autoWB)
        frame = adjustColorTemperature(frame,wb);
    frame = ::adjustSize(frame,1.0);
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // OpenCV 默认 BGR，需要转换为 RGB
    QPixmap pixmap = QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888));
    videoLabel->setPixmap(pixmap.scaled(videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::on_Action_File_Set_Capture_Directory(){
    // 获取当前工作目录作为对话框的起始目录
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Folder"), QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        video_saved_path = dir;
        qDebug() << "video_saved_path:" << video_saved_path;
    }
}

void MainWindow::on_Action_File_Set_Photo_Directory(){
    // 获取当前工作目录作为对话框的起始目录
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Folder"), QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // 如果用户选择了一个文件夹，输出到调试控制台
    if (!dir.isEmpty()) {
        image_saved_path = dir;
        qDebug() << "image_saved_path:" << image_saved_path;
    }
}

void MainWindow::on_Action_File_Exit(){
    this->close();
}

void MainWindow::on_Action_Option_Video_Capture_Filter(){
    Get_Amp_Default_Values(); // 获取当前摄像头参数值
    video_capture_filter = new VideoCaptureFilter(this);
    qDebug() << brightness << contrast << hue << saturability << plain << gamma << backlight << gain;
    video_capture_filter->Set_Amp_Default_Values(brightness, contrast, hue, saturability, plain, gamma, backlight, gain);
    video_capture_filter->setWindowModality(Qt::WindowModal); // 设置为模态对话框
    video_capture_filter->show();
    //算法相关
    //video_capture_filter = new VideoCaptureFilter(this);
    connect(video_capture_filter, &VideoCaptureFilter::brightnessChanged, this, &MainWindow::onVideoCaptureFilterBrightnessChanged);
    connect(video_capture_filter, &VideoCaptureFilter::contrastChanged, this, &MainWindow::onVideoCaptureFilterContrastChanged);
    connect(video_capture_filter, &VideoCaptureFilter::hueChanged, this, &MainWindow::onVideoCaptureFilterHueChanged);
    connect(video_capture_filter, &VideoCaptureFilter::saturabilityChanged, this, &MainWindow::onVideoCaptureFilterSaturabilityChanged);
    connect(video_capture_filter, &VideoCaptureFilter::plainChanged, this, &MainWindow::onVideoCaptureFilterPlainChanged);
    connect(video_capture_filter, &VideoCaptureFilter::gammaChanged, this, &MainWindow::onVideoCaptureFilterGammaChanged);
    connect(video_capture_filter, &VideoCaptureFilter::wbChanged, this, &MainWindow::onVideoCaptureFilterWbChanged);
    connect(video_capture_filter, &VideoCaptureFilter::backlightChanged, this, &MainWindow::onVideoCaptureFilterBacklightChanged);
    connect(video_capture_filter, &VideoCaptureFilter::gainChanged, this, &MainWindow::onVideoCaptureFilterGainChanged);
}

void MainWindow::on_Action_Option_Video_Capture_pin(){
    return;
}

// 截图
void MainWindow::on_Action_Capture_Capture_Image(){
    if (image_saved_path.isEmpty()) {
        QMessageBox::information(this, "Warning", "The image save path is empty. Please specify a valid path.");
        on_Action_File_Set_Photo_Directory();
    } else{
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        QString filePath = QDir(image_saved_path).filePath(QString("image_%1.png").arg(timestamp));
        videoLabel->pixmap().save(filePath);
    }
}

// 开始录制视频
void MainWindow::startVideoRecording() {
    if (isRecording) {
        return;  // 如果已经在录制，则不重复开始
    }
    if(video_saved_path.isEmpty()){
        QMessageBox::information(this, "Warning", "The video save path is empty. Please specify a valid path.");
        on_Action_File_Set_Capture_Directory();
    }else{
        // 打开VideoWriter，设置视频保存路径，编码格式，帧率和尺寸
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        QString filePath = QDir(video_saved_path).filePath(QString("video_%1.avi").arg(timestamp));
        writer.open(filePath.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(videoLabel->pixmap().size().width(), videoLabel->pixmap().size().height()), true);
        if (!writer.isOpened()) {
            qDebug() << "Error opening video writer!";
            return;
        }
        // 开始录制
        isRecording = true;

        recordTimer = new QTimer(this);
        connect(recordTimer, &QTimer::timeout, this, [=]() {
            QImage img = videoLabel->pixmap().toImage();
            if (img.format() != QImage::Format_RGB888) {
                img = img.convertToFormat(QImage::Format_RGB888);
            }
            cv::Mat frame(videoLabel->pixmap().size().height(), videoLabel->pixmap().size().width(), CV_8UC3, img.bits(), img.bytesPerLine());
            frame = frame.clone();  // 确保 OpenCV Mat 有独立的内存
            // 转换为BGR格式
            cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);  // RGB -> BGR转换
            writer.write(frame);
        });

        // 每隔一段时间调用timeout槽来抓取视频帧，确保定时器的频率与帧率匹配
        recordTimer->start(1000 / fps);  // 以帧率的反时间（毫秒）启动定时器
    }

}

// 停止录制视频
void MainWindow::stopVideoRecording() {
    if (!isRecording) {
        return;  // 如果当前没有在录制，则直接返回
    }

    // 停止定时器
    if (recordTimer) {
        recordTimer->stop();
        delete recordTimer;
        recordTimer = nullptr;
    }

    // 释放VideoWriter资源
    writer.release();

    // 标记录制状态为停止
    isRecording = false;
}

void MainWindow::on_Action_Capture_Capture_Audio(){
    return;
}

void MainWindow::on_Action_Capture_Capture_Buffer_Data(){
    return;
}

void MainWindow::on_Action_Capture_Capture_Prevciew_Picture(){
    return;
}

void MainWindow::on_Action_Capture_Set_Frame_Rate(){
    return;
}

void MainWindow::on_Action_Capture_Set_Time_Limit(){
    return;
}

void MainWindow::on_Action_Capture_Set_Video_Compressor(){
    return;
}

void MainWindow::on_Action_Capture_Master_Stream_None(){
    return;
}

void MainWindow::on_Action_Capture_Master_Stream_Video(){
    return;
}

void MainWindow::on_Action_Capture_Master_Stream_Audio(){
    return;
}

void MainWindow::on_Action_Photo_Start_Photo_F3(){
    return;
}

void MainWindow::on_Action_Photo_Set_Photo_Format(){
    return;
}

void MainWindow::on_Action_Help_About(){
    return;
}


void MainWindow::Get_Amp_Default_Values(){
    brightness = cap.get(CAP_PROP_BRIGHTNESS);
    contrast = cap.get(CAP_PROP_CONTRAST);
    hue = cap.get(CAP_PROP_HUE);
    saturability = cap.get(CAP_PROP_SATURATION);
    plain = cap.get(CAP_PROP_FOCUS);
    gamma = cap.get(CAP_PROP_GAMMA);
    backlight = cap.get(CAP_PROP_BACKLIGHT);
    gain = cap.get(CAP_PROP_GAIN);
}

void MainWindow::onVideoCaptureFilterBrightnessChanged(int value) {
    // 对主窗口中视频帧进行亮度操作...
    // brightness = value;
    cap.set(CAP_PROP_BRIGHTNESS,value);
    qDebug() << "brightness changed to " << value;
}

void MainWindow::onVideoCaptureFilterContrastChanged(int value) {
    // 对主窗口中视频帧进行对比度操作...
    // contrast = value;
    cap.set(CAP_PROP_CONTRAST,value);
    qDebug() << "contrast changed to " << value;
}

void MainWindow::onVideoCaptureFilterHueChanged(int value) {
    // 对主窗口中视频帧进行色调操作...
    // hue =value;
    cap.set(CAP_PROP_HUE,value);
    qDebug() << "hue changed to " << value;
}

void MainWindow::onVideoCaptureFilterSaturabilityChanged(int value) {
    // 对主窗口中视频帧进行饱和度操作...
    // saturability = value;
    cap.set(CAP_PROP_SATURATION,value);
    qDebug() << "saturability changed to " << value;
}

void MainWindow::onVideoCaptureFilterPlainChanged(int value) {
    // 对主窗口中视频帧进行清晰度操作...
    // plain = value;
    cap.set(CAP_PROP_FOCUS,value);
    qDebug() << "plain changed to " << value;
}

void MainWindow::onVideoCaptureFilterGammaChanged(int value) {
    // 对主窗口中视频帧进行Gamma操作...
    cap.set(CAP_PROP_GAMMA,value);
    qDebug() << "gamma changed to " << value;
}

void MainWindow::onVideoCaptureFilterWbChanged(int value) {
    // 对主窗口中视频帧进行白平衡操作...
    wb =value;
    if(WB_ABILITY)
        cap.set(CAP_PROP_TEMPERATURE,value);
    qDebug() << "wb changed to " << value;
    // qDebug()<< cap.get(CAP_PROP_WHITE_BALANCE_BLUE_U);
    // qDebug()<< cap.get(CAP_PROP_WHITE_BALANCE_RED_V);
    // qDebug()<< cap.get(CAP_PROP_WB_TEMPERATURE);
    // qDebug()<< cap.get(CAP_PROP_TEMPERATURE);
    // qDebug()<< cap.get(CAP_PROP_AUTO_WB);

}

void MainWindow::onVideoCaptureFilterBacklightChanged(int value) {
    // 对主窗口中视频帧进行逆光对比操作...
    cap.set(CAP_PROP_BACKLIGHT,value);
    qDebug() << "backlight changed to " << value;
}

void MainWindow::onVideoCaptureFilterGainChanged(int value) {
    // 对主窗口中视频帧进行增益操作...
    cap.set(CAP_PROP_GAIN,value);
    qDebug() << "gain changed to " << value ;
}

void MainWindow::onVideoCaptureFilterAutoWBChanged(bool value)
{
    autoWB = value;
    qDebug()<< value;
}


