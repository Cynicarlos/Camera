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

    //Files
    QAction *action_File_Set_Capture_Directory = ui->action_File_Set_Capture_Directory;
    QAction *action_File_Set_Photo_Directory = ui->action_File_Set_Photo_Directory;
    QAction *action_File_Save_Captured_Video_As = ui->action_File_Save_Captured_Video_As;
    QAction *action_File_Exit = ui->action_File_Exit;

    //Options
    QAction *action_Option_Preview = ui->action_Option_Preview;
    QAction *action_Option_Video_Capture_Filter = ui->action_Option_Video_Capture_Filter;
    QAction *action_Option_Video_Capture_pin = ui->action_Option_Video_Capture_pin;

    //Capture
    QAction *action_Capture_Capture_Audio = ui->action_Capture_Capture_Audio;
    QAction *action_Capture_Capture_Buffer_Data = ui->action_Capture_Capture_Buffer_Data;
    QAction *action_Capture_Capture_Prevciew_Picture = ui->action_Capture_Capture_Prevciew_Picture;
    QAction *action_Capture_Set_Frame_Rate = ui->action_Capture_Set_Frame_Rate;
    QAction *action_Capture_Set_Time_Limit = ui->action_Capture_Set_Time_Limit;
    QAction *action_Capture_Set_Video_Compressor = ui->action_Capture_Set_Video_Compressor;
    QAction *action_Capture_Start_Capture = ui->action_Capture_Start_Capture;
    QAction *action_Capture_Stop_Capture_ESC = ui->action_Capture_Stop_Capture_ESC;

    QAction *action_Capture_Master_Stream_None = ui->action_Capture_Master_Stream_None;
    QAction *action_Capture_Master_Stream_Video = ui->action_Capture_Master_Stream_Video;
    QAction *action_Capture_Master_Stream_Audio = ui->action_Capture_Master_Stream_Audio;

    //Photo
    QAction *action_Photo_Start_Photo_F3 = ui->action_Photo_Start_Photo_F3;
    QAction *action_Photo_Set_Photo_Format = ui->action_Photo_Set_Photo_Format;

    //Help
    QAction *action_Help_About = ui->action_Help_About;

    connect(action_File_Set_Capture_Directory, &QAction::triggered, this, &MainWindow::on_Action_File_Set_Capture_Directory);
    connect(action_File_Set_Photo_Directory, &QAction::triggered, this, &MainWindow::on_Action_File_Set_Photo_Directory);
    connect(action_File_Save_Captured_Video_As, &QAction::triggered, this, &MainWindow::on_Action_File_Save_Captured_Video_As);
    connect(action_File_Exit, &QAction::triggered, this, &MainWindow::on_Action_File_Exit);

    connect(action_Option_Preview, &QAction::triggered, this, &MainWindow::on_Action_Option_Preview);
    connect(action_Option_Video_Capture_Filter, &QAction::triggered, this, &MainWindow::on_Action_Option_Video_Capture_Filter);
    connect(action_Option_Video_Capture_pin, &QAction::triggered, this, &MainWindow::on_Action_Option_Video_Capture_pin);

    connect(action_Capture_Capture_Audio, &QAction::triggered, this, &MainWindow::on_Action_Capture_Capture_Audio);
    connect(action_Capture_Capture_Buffer_Data, &QAction::triggered, this, &MainWindow::on_Action_Capture_Capture_Buffer_Data);
    connect(action_Capture_Capture_Prevciew_Picture, &QAction::triggered, this, &MainWindow::on_Action_Capture_Capture_Prevciew_Picture);
    connect(action_Capture_Set_Frame_Rate, &QAction::triggered, this, &MainWindow::on_Action_Capture_Set_Frame_Rate);
    connect(action_Capture_Set_Time_Limit, &QAction::triggered, this, &MainWindow::on_Action_Capture_Set_Time_Limit);
    connect(action_Capture_Set_Video_Compressor, &QAction::triggered, this, &MainWindow::on_Action_Capture_Set_Video_Compressor);
    connect(action_Capture_Start_Capture, &QAction::triggered, this, &MainWindow::on_Action_Capture_Start_Capturen);
    connect(action_Capture_Stop_Capture_ESC, &QAction::triggered, this, &MainWindow::on_Action_Capture_Stop_Capture_ESC);
    connect(action_Capture_Master_Stream_None, &QAction::triggered, this, &MainWindow::on_Action_Capture_Master_Stream_None);
    connect(action_Capture_Master_Stream_Video, &QAction::triggered, this, &MainWindow::on_Action_Capture_Master_Stream_Video);
    connect(action_Capture_Master_Stream_Audio, &QAction::triggered, this, &MainWindow::on_Action_Capture_Master_Stream_Audio);

    connect(action_Photo_Start_Photo_F3, &QAction::triggered, this, &MainWindow::on_Action_Photo_Start_Photo_F3);
    connect(action_Photo_Set_Photo_Format, &QAction::triggered, this, &MainWindow::on_Action_Photo_Set_Photo_Format);

    connect(action_Help_About, &QAction::triggered, this, &MainWindow::on_Action_Help_About);

    // 创建菜单栏
    menuBar = ui->menubar;

    videoLabel = ui->videoLabel;
    videoLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
}

MainWindow::~MainWindow()
{
    cap.release(); // 释放摄像头资源
    delete ui;
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
// void MainWindow::updateBrightness(int value)
// {
//     brightness = value; // 更新亮度值
// }

// void MainWindow::updateContrast(int value)
// {
//     contrast = value; // 更新对比度值
// }

void MainWindow::on_Action_File_Set_Capture_Directory(){
    // 获取当前工作目录作为对话框的起始目录
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Folder"), QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // 如果用户选择了一个文件夹，输出到调试控制台
    if (!dir.isEmpty()) {
        qDebug() << "Selected folder:" << dir;
        // 这里可以进行其他操作，比如设置文件夹路径到某个成员变量等
    }
}
void MainWindow::on_Action_File_Set_Photo_Directory(){
    // 获取当前工作目录作为对话框的起始目录
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Folder"), QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // 如果用户选择了一个文件夹，输出到调试控制台
    if (!dir.isEmpty()) {
        qDebug() << "Selected folder:" << dir;
        // 这里可以进行其他操作，比如设置文件夹路径到某个成员变量等
    }
}
void MainWindow::on_Action_File_Save_Captured_Video_As(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(),
                                                    tr("Text Files (*.txt);;All Files (*)"));

    // 如果用户选择了一个文件名，输出到调试控制台
    if (!fileName.isEmpty()) {
        qDebug() << "Selected file:" << fileName;
        // 这里可以进行保存文件的操作
        // 例如：writeToFile(fileName);
    }
}
void MainWindow::on_Action_File_Exit(){
    this->close();
}


void MainWindow::on_Action_Option_Preview(){
    // 初始化 OpenCV 摄像头
    cap.open(0); // 使用默认摄像头
    if (!cap.isOpened()) {
        qFatal("Failed to open camera");
        return;
    }
    // 启动定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(30); // 每 30 毫秒更新一次（约 33 FPS）
}
void MainWindow::on_Action_Option_Video_Capture_Filter(){

    VideoCaptureFilter *video_capture_filter = new VideoCaptureFilter(this);
    video_capture_filter->setWindowModality(Qt::WindowModal); // 设置为模态对话框
    video_capture_filter->show();
}

void MainWindow::on_Action_Option_Video_Capture_pin(){
    return;
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
void MainWindow::on_Action_Capture_Start_Capturen(){
    return;
}
void MainWindow::on_Action_Capture_Stop_Capture_ESC(){
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

