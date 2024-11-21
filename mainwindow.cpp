#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    // , brightness(0)
    // , contrast(100)
{
    ui->setupUi(this);

    this->setStyleSheet("QMenu::item:selected {background-color:rgb(234, 243, 253);}");
    init();
    init_Camera();

    // 创建菜单栏
    menuBar = ui->menubar;
    videoLabel = ui->videoLabel;
    videoLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    videoLabel->setScaledContents(true); // 保持图片比例
    videoLabel->setAlignment(Qt::AlignCenter); // 居中显示videoLabel = ui->videoLabel;

}

MainWindow::~MainWindow()
{
    cap.release(); // 释放摄像头资源lk.
    delete ui;
}

void MainWindow::init(){
    //Files
    action_File_Set_Capture_Directory = ui->action_File_Set_Video_Directory;
    action_File_Set_Photo_Directory = ui->action_File_Set_Image_Directory;
    action_File_Save_Captured_Video_As = ui->action_File_Save_Captured_Video_As;
    action_File_Exit = ui->action_File_Exit;

    //Options
    //action_Option_Preview = ui->action_Option_Preview;
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
    //connect(action_File_Save_Captured_Video_As, &QAction::triggered, this, &MainWindow::on_Action_File_Save_Captured_Video_As);
    connect(action_File_Exit, &QAction::triggered, this, &MainWindow::on_Action_File_Exit);

    //connect(action_Option_Preview, &QAction::triggered, this, &MainWindow::on_Action_Option_Preview);
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

QSize MainWindow::get_Max_Resolution(QList<QSize> resolutions) {
    if (resolutions.isEmpty()) {
        return QSize(); // 或者抛出一个异常，或者返回一个错误代码
    }

    QSize maxResolution = resolutions.first(); // 使用列表中的第一个分辨率作为初始值
    for (const QSize &res : resolutions) {
        if (res.width() * res.height() > maxResolution.width() * maxResolution.height()) {
            maxResolution = res;
        }
    }
    return maxResolution;
}
void MainWindow::init_Camera() {
    menu_devices = ui->menuDevices;
    cameras = std::make_unique<QList<QCameraDevice>>(QMediaDevices::videoInputs());
    QList<QSize> resolutions; //当前摄像头支持的分辨率

    // 遍历所有视频输入设备
    for (int i = 0; i < cameras->size(); ++i) {
        QCameraDevice cameraDevice = cameras->at(i);
        QString deviceDescription = cameraDevice.description();

        // 创建一个可勾选的 QAction
        QAction *cameraAction = new QAction(deviceDescription, menu_devices);
        cameraAction->setCheckable(true);  // 设置为可勾选
        resolutions = cameraDevice.photoResolutions();//当前相机支持的分辨率
        if(i == 0){
            cameraAction->setChecked(true);//给默认设备打勾
            max_resolution = get_Max_Resolution(resolutions);
        }
        menu_devices->addAction(cameraAction);

        // 连接信号和槽
        connect(cameraAction, &QAction::triggered, this, [this, i, cameraAction, resolutions]() {
            on_Devices_Selected(i, cameraAction, resolutions);  // 传递设备ID和对应的 QAction
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
void MainWindow::on_Devices_Selected(int selected_deviceId, QAction *selectedAction, QList<QSize> resolutions) {
    if(device_id == selected_deviceId) return;
    // 打开另一个摄像头获取最大分辨率
    cap.open(selected_deviceId);
    if (!cap.isOpened()) {
        qFatal("Failed to open camera");
        return;
    }
    max_resolution = get_Max_Resolution(resolutions);
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

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // OpenCV 默认 BGR，需要转换为 RGB
    QPixmap pixmap = QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888));
    videoLabel->setPixmap(pixmap.scaled(videoLabel->size(), Qt::KeepAspectRatio));
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

    VideoCaptureFilter *video_capture_filter = new VideoCaptureFilter(this);
    video_capture_filter->setWindowModality(Qt::WindowModal); // 设置为模态对话框
    video_capture_filter->show();
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


// void MainWindow::startVideoRecording() {
//     if (video_saved_path.isEmpty()) {
//         QMessageBox::information(this, "Warning", "The video save path is empty. Please specify a valid path.");
//         on_Action_File_Set_Capture_Directory();
//     } else{
//         // 创建 VideoRecorder 实例并设置 QLabel
//         videorecorder = new VideoRecorder(this);
//         videorecorder->setLabel(videoLabel);

//         // 设置保存视频的路径
//         QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
//         QString filePath = QDir(video_saved_path).filePath(QString("video_%1.mp4").arg(timestamp));
//         //QString outputFilePath = QDir::homePath() + "/output_video.mp4";
//         // 开始录制
//         videorecorder->startRecording(filePath);
//     }
// }

// void MainWindow::stopVideoRecording() {
//     if (videorecorder) {
//         videorecorder->stopRecording();
//     }
// }



// void MainWindow::onVideoFrameChanged(const QVideoFrame &frame)
// {
//     //qDebug() << "Video frame received";  // 每次接收到视频帧时输出调试信息

//     // 转换 QVideoFrame 为 QImage
//     QImage image = frame.toImage();
//     if (!image.isNull()) {
//         processImage(image);  // 处理捕获的图像
//     } else {
//         qDebug() << "Captured video frame is null!";
//     }
// }


// void MainWindow::processImage(const QImage &inputImage)
// {
//     if (!inputImage.isNull()) {

//         QImage hq_image = inputImage.convertToFormat(QImage::Format_Grayscale8);  // 这里可以插入实际的图像处理逻辑
//         //=======================================
//         // cv::Mat lq_img = QImage2cvMat(inputImage);
//         // cv::cvtColor(lq_img, lq_img, cv::COLOR_BGR2RGB);
//         // cv::resize(lq_img, lq_img, cv::Size(224, 224));
//         // torch::Tensor tensor = torch::from_blob(lq_img.data, {1, 224, 224, 3}, torch::kByte);
//         // tensor = tensor.permute({0, 3, 1, 2});
//         // tensor = tensor.to(torch::kFloat);
//         // tensor = tensor.div(255);

//         // torch::jit::IValue input = tensor.unsqueeze(0);
//         // at::Tensor output = model.forward({input}).toTensor();

//         //MAT -> qImage
//         // QImage hq_image = cvMat2QImage(lq_img);
//         //=======================================

//         //qDebug() << "Processed image size:" << processedImage.size(); // 输出处理后图像的大小

//         // 将处理后的图像转换为 QPixmap
//         QPixmap pixmap = QPixmap::fromImage(hq_image);

//         // 更新 QLabel，显示处理后的图像
//         m_processedVideoWidget->setPixmap(pixmap.scaled(m_originalVideoWidget->size(), Qt::KeepAspectRatio));

//         // 强制刷新 QLabel，确保图像显示
//         //m_processedVideoWidget->repaint();
//     } else {
//         qDebug() << "Processed image is null!";
//     }
// }


// cv::Mat QImage2cvMat(const QImage &image){
//     cv::Mat mat;
//     switch(image.format()){
//         case QImage::Format_Grayscale8: // 灰度图，每个像素点1个字节（8位）
//             // Mat构造：行数，列数，存储结构，数据，step每行多少字节
//             mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
//             break;
//         case QImage::Format_ARGB32: // uint32存储0xAARRGGBB，pc一般小端存储低位在前，所以字节顺序就成了BGRA
//         case QImage::Format_RGB32: // Alpha为FF
//         case QImage::Format_ARGB32_Premultiplied:
//             mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
//             break;
//         case QImage::Format_RGB888: // RR,GG,BB字节顺序存储
//             mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
//             // opencv需要转为BGR的字节顺序
//             cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
//             break;
//         case QImage::Format_RGBA64: // uint64存储，顺序和Format_ARGB32相反，RGBA
//             mat = cv::Mat(image.height(), image.width(), CV_16UC4, (void*)image.constBits(), image.bytesPerLine());
//             // opencv需要转为BGRA的字节顺序
//             cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA);
//             break;
//         default:
//             break;
//     }
//     return mat;
// }

// QImage cvMat2QImage(const cv::Mat &mat)
// {
//     QImage image;
//     switch(mat.type())
//     {
//     case CV_8UC1:
//         // QImage构造：数据，宽度，高度，每行多少字节，存储结构
//         image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
//         break;
//     case CV_8UC3:
//         image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
//         image = image.rgbSwapped(); // BRG转为RGB
//         // Qt5.14增加了Format_BGR888
//         // image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.cols * 3, QImage::Format_BGR888);
//         break;
//     case CV_8UC4:
//         image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
//         break;
//     case CV_16UC4:
//         image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGBA64);
//         image = image.rgbSwapped(); // BRG转为RGB
//         break;
//     }
//     return image;
// }

// void MainWindow::_setup_devices_display()
// {
//     QVector<QString> loadedCameraDevices;
//     for (const QCameraDevice &info : QMediaDevices::videoInputs())
//     {
//         QString deviceName = info.description();
//         loadedCameraDevices.append(deviceName);
//         ui->available_camera_devices_list->addItem(deviceName);
//     }
// }

// void MainWindow::on_available_camera_devices_list_itemClicked(QListWidgetItem *item)
// {
//     // 获取被点击的相机名称
//     QString selectedCameraName = item->text();

//     // 检查当前的相机是否已经在使用，如果是则直接返回
//     if (m_camera && m_camera->cameraDevice().description() == selectedCameraName)
//     {
//         return;  // 当前相机正在使用，跳过不处理
//     }

//     // 遍历可用的相机设备，匹配选中的相机名称
//     for (const QCameraDevice &cameraDevice : QMediaDevices::videoInputs())
//     {
//         if (cameraDevice.description() == selectedCameraName)
//         {
//             // 停止当前摄像头并释放
//             if (m_camera)
//             {
//                 m_camera->stop();
//             }

//             // 创建新的 QCamera 对象并设置新的相机设备
//             m_camera.reset(new QCamera(cameraDevice));
//             m_CaptureSession.setCamera(m_camera.data());
//             m_CaptureSession.setVideoOutput(m_originalVideoWidget);

//             // 启动新相机
//             m_camera->start();
//             ui->switch_button->setChecked(true);
//             ui->switch_button->setText("On");

//             break;  // 找到并启动相机后跳出循环
//         }
//     }
// }



// void MainWindow::_setup_ui()
// {
//     ui->switch_button->setChecked(true);
// }

// void MainWindow::on_quit_app_button_clicked()
// {
//     QApplication::exit();
// }

// void MainWindow::on_switch_button_clicked(bool checked)
// {
//     if (checked) {
//         m_camera->start();
//         ui->switch_button->setChecked(true);
//         ui->switch_button->setText("On");
//     }
//     else {
//         m_camera->stop();
//         ui->switch_button->setChecked(false);
//         ui->switch_button->setText("Off");
//     }
// }

