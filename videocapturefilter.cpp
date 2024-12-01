#include "videocapturefilter.h"

VideoCaptureFilter::VideoCaptureFilter(QWidget *parent) : QDialog(parent), ui(new Ui::VideoCaptureFilter)
{
    ui->setupUi(this);
    init_Widget();
    set_General_Slots();
    bind_Slider_Spinbox();
    Set_Camera_Control_Default_Values();
}

VideoCaptureFilter::~VideoCaptureFilter()
{
    delete ui;
}

void VideoCaptureFilter::init_Widget()
{
    pushButton_amp_default = ui->pushButton_amp_default;
    pushButton_amp_yes = ui->pushButton_amp_yes;
    pushButton_amp_cancel = ui->pushButton_amp_cancel;
    pushButton_camera_control_default = ui->pushButton_camera_control_default;
    pushButton_camera_control_yes = ui->pushButton_camera_control_yes;
    pushButton_camera_control_cancel = ui->pushButton_camera_control_cancel;

    Slider_brightness = ui->Slider_brightness;
    spinBox_brightness = ui->spinBox_brightness;
    checkBox_auto_brightness = ui->checkBox_auto_brightness;
    connect(Slider_brightness, &QSlider::valueChanged, this, &VideoCaptureFilter::onBrightnessChanged);

    Slider_contrast = ui->Slider_contrast;
    spinBox_contrast = ui->spinBox_contrast;
    checkBox_auto_contrast = ui->checkBox_auto_contrast;
    connect(Slider_contrast, &QSlider::valueChanged, this, &VideoCaptureFilter::onContrastChanged);


    Slider_hue = ui->Slider_hue;
    spinBox_hue = ui->spinBox_hue;
    checkBox_auto_hue = ui->checkBox_auto_hue;
    connect(Slider_hue, &QSlider::valueChanged, this, &VideoCaptureFilter::onHueChanged);

    Slider_saturability = ui->Slider_saturability;
    spinBox_saturability = ui->spinBox_saturability;
    checkBox_auto_saturability = ui->checkBox_auto_saturability;
    connect(Slider_saturability, &QSlider::valueChanged, this, &VideoCaptureFilter::onSaturabilityChanged);

    Slider_plain = ui->Slider_plain;
    spinBox_plain = ui->spinBox_plain;
    checkBox_auto_plain = ui->checkBox_auto_plain;
    connect(Slider_plain, &QSlider::valueChanged, this, &VideoCaptureFilter::onPlainChanged);

    Slider_gamma = ui->Slider_gamma;
    spinBox_gamma = ui->spinBox_gamma;
    checkBox_auto_gamma = ui->checkBox_auto_gamma;
    connect(Slider_gamma, &QSlider::valueChanged, this, &VideoCaptureFilter::onGammaChanged);

    Slider_wb = ui->Slider_wb;
    spinBox_wb = ui->spinBox_wb;
    checkBox_auto_wb = ui->checkBox_auto_wb;
    connect(Slider_wb, &QSlider::valueChanged, this, &VideoCaptureFilter::onWbChanged);
    connect(checkBox_auto_wb,&QCheckBox::checkStateChanged,this, &VideoCaptureFilter::onAutoWBChanged);

    Slider_backlight = ui->Slider_backlight;
    spinBox_backlight = ui->spinBox_backlight;
    checkBox_auto_backlight = ui->checkBox_auto_backlight;
    connect(Slider_backlight, &QSlider::valueChanged, this, &VideoCaptureFilter::onBacklightChanged);

    Slider_gain = ui->Slider_gain;
    spinBox_gain = ui->spinBox_gain;
    checkBox_auto_gain = ui->checkBox_auto_gain;
    connect(Slider_gain, &QSlider::valueChanged, this, &VideoCaptureFilter::onGainChanged);

    //===========================camera control===========================
    Slider_zoom = ui->Slider_zoom;
    spinBox_zoom = ui->spinBox_zoom;
    checkBox_auto_zoom = ui->checkBox_auto_zoom;

    Slider_focus = ui->Slider_focus;
    spinBox_focus = ui->spinBox_focus;
    checkBox_auto_focus = ui->checkBox_auto_focus;

    Slider_exposure = ui->Slider_exposure;
    spinBox_exposure = ui->spinBox_exposure;
    checkBox_auto_exposure = ui->checkBox_auto_exposure;

    Slider_aperture = ui->Slider_aperture;
    spinBox_aperture = ui->spinBox_aperture;
    checkBox_auto_aperture = ui->checkBox_auto_aperture;

    Slider_panorama = ui->Slider_panorama;
    spinBox_panorama = ui->spinBox_panorama;
    checkBox_auto_panorama = ui->checkBox_auto_panorama;

    Slider_tilt = ui->Slider_tilt;
    spinBox_tilt = ui->spinBox_tilt;
    checkBox_auto_tilt = ui->checkBox_auto_tilt;

    Slider_roll = ui->Slider_roll;
    spinBox_roll = ui->spinBox_roll;
    checkBox_auto_roll = ui->checkBox_auto_roll;
}


void VideoCaptureFilter::Set_Amp_Default_Values(int b, double c, int h, int s, int p, double g, int bl, int gN){
    qDebug() << brightness << contrast << hue << saturability << plain << gamma << backlight << gain;
    brightness = b;
    contrast = c;
    hue = h;
    saturability = s;
    plain = p;
    gamma = g;
    backlight = bl;
    gain = gN;

    Slider_brightness->setSliderPosition(brightness);
    spinBox_brightness->setValue(brightness);
    checkBox_auto_brightness->setCheckable(false);

    Slider_contrast->setSliderPosition(contrast);
    spinBox_contrast->setValue(contrast);
    checkBox_auto_contrast->setCheckable(false);

    Slider_hue->setSliderPosition(hue);
    spinBox_hue->setValue(hue);
    checkBox_auto_hue->setCheckable(false);

    Slider_saturability->setSliderPosition(saturability);
    spinBox_saturability->setValue(saturability);
    checkBox_auto_saturability->setCheckable(false);

    Slider_plain->setSliderPosition(plain);
    spinBox_plain->setValue(plain);
    checkBox_auto_plain->setCheckable(false);

    Slider_gamma->setSliderPosition(gamma);
    spinBox_gamma->setValue(gamma);
    checkBox_auto_gamma->setCheckable(false);

    //Slider_wb->setSliderPosition(4000);
    //spinBox_wb->setValue(4000);
    checkBox_auto_wb->setCheckable(true);
    checkBox_auto_wb->setChecked(false);//这里要加一个自动白平衡处理

    Slider_backlight->setSliderPosition(backlight);
    spinBox_backlight->setValue(backlight);
    checkBox_auto_backlight->setCheckable(false);

    Slider_gain->setSliderPosition(gain);
    spinBox_gain->setValue(gain);
    checkBox_auto_gain->setCheckable(false);

}

void VideoCaptureFilter::Amp_Change2Default_Values(){
    Slider_brightness->setSliderPosition(brightness);
    spinBox_brightness->setValue(brightness);
    checkBox_auto_brightness->setCheckable(false);

    Slider_contrast->setSliderPosition(contrast);
    spinBox_contrast->setValue(contrast);
    checkBox_auto_contrast->setCheckable(false);

    Slider_hue->setSliderPosition(hue);
    spinBox_hue->setValue(hue);
    checkBox_auto_hue->setCheckable(false);

    Slider_saturability->setSliderPosition(saturability);
    spinBox_saturability->setValue(saturability);
    checkBox_auto_saturability->setCheckable(false);

    Slider_plain->setSliderPosition(plain);
    spinBox_plain->setValue(plain);
    checkBox_auto_plain->setCheckable(false);

    Slider_gamma->setSliderPosition(gamma);
    spinBox_gamma->setValue(gamma);
    checkBox_auto_gamma->setCheckable(false);

    //Slider_wb->setSliderPosition(4000);
    //spinBox_wb->setValue(4000);
    checkBox_auto_wb->setCheckable(true);
    checkBox_auto_wb->setChecked(false);//这里要加一个自动白平衡处理

    Slider_backlight->setSliderPosition(backlight);
    spinBox_backlight->setValue(backlight);
    checkBox_auto_backlight->setCheckable(false);

    Slider_gain->setSliderPosition(gain);
    spinBox_gain->setValue(gain);
    checkBox_auto_gain->setCheckable(false);

}


void VideoCaptureFilter::Set_Camera_Control_Default_Values(){

    //==============================camera control=======================
    Slider_zoom->setSliderPosition(100);
    spinBox_zoom->setValue(100);
    checkBox_auto_zoom->setCheckable(false);

    Slider_focus->setSliderPosition(0);
    spinBox_focus->setValue(0);
    checkBox_auto_focus->setCheckable(false);

    //Slider_exposure->setSliderPosition(0);
    //spinBox_exposure->setValue(0);
    checkBox_auto_exposure->setCheckable(true);
    checkBox_auto_exposure->setChecked(true);//这里是一个自动曝光处理

    Slider_aperture->setSliderPosition(0);
    spinBox_aperture->setValue(0);
    checkBox_auto_aperture->setCheckable(false);

    Slider_panorama->setSliderPosition(0);
    spinBox_panorama->setValue(0);
    checkBox_auto_panorama->setCheckable(false);

    Slider_tilt->setSliderPosition(0);
    spinBox_tilt->setValue(0);
    checkBox_auto_tilt->setCheckable(false);

    Slider_roll->setSliderPosition(0);
    spinBox_roll->setValue(0);
    checkBox_auto_roll->setCheckable(false);

}

void VideoCaptureFilter::on_Amp_Yes(){
    //这里是用户给定视频参数，调整视频并显示的逻辑
    this->close();
}

void VideoCaptureFilter::on_Amp_Cannel(){
    this->close();
}

void VideoCaptureFilter::on_Camera_Control_Yes(){
    //这里是用户给定相机参数，调整视频并显示的逻辑
    this->close();
}

void VideoCaptureFilter::on_Camera_Control_Cannel(){
    this->close();
}

void VideoCaptureFilter::set_General_Slots(){
    connect(pushButton_amp_default, &QPushButton::clicked, this, &VideoCaptureFilter::Amp_Change2Default_Values);

    connect(pushButton_amp_yes, &QPushButton::clicked, this, &VideoCaptureFilter::on_Amp_Yes);

    connect(pushButton_amp_cancel, &QPushButton::clicked, this, &VideoCaptureFilter::on_Amp_Cannel);

    connect(pushButton_camera_control_default, &QPushButton::clicked, this, &VideoCaptureFilter::Set_Camera_Control_Default_Values);

    connect(pushButton_camera_control_yes, &QPushButton::clicked, this, &VideoCaptureFilter::on_Camera_Control_Yes);

    connect(pushButton_camera_control_cancel, &QPushButton::clicked, this, &VideoCaptureFilter::on_Camera_Control_Cannel);
}

void VideoCaptureFilter::bind_Slider_Spinbox(){

    QObject::connect(Slider_brightness, &QSlider::valueChanged, spinBox_brightness, &QSpinBox::setValue);
    QObject::connect(spinBox_brightness, &QSpinBox::valueChanged, Slider_brightness, &QSlider::setValue);

    QObject::connect(Slider_contrast, &QSlider::valueChanged, spinBox_contrast, &QSpinBox::setValue);
    QObject::connect(spinBox_contrast, &QSpinBox::valueChanged, Slider_contrast, &QSlider::setValue);

    QObject::connect(Slider_hue, &QSlider::valueChanged, spinBox_hue, &QSpinBox::setValue);
    QObject::connect(spinBox_hue, &QSpinBox::valueChanged, Slider_hue, &QSlider::setValue);

    QObject::connect(Slider_saturability, &QSlider::valueChanged, spinBox_saturability, &QSpinBox::setValue);
    QObject::connect(spinBox_saturability, &QSpinBox::valueChanged, Slider_saturability, &QSlider::setValue);

    QObject::connect(Slider_plain, &QSlider::valueChanged, spinBox_plain, &QSpinBox::setValue);
    QObject::connect(spinBox_plain, &QSpinBox::valueChanged, Slider_plain, &QSlider::setValue);

    QObject::connect(Slider_gamma, &QSlider::valueChanged, spinBox_gamma, &QSpinBox::setValue);
    QObject::connect(spinBox_gamma, &QSpinBox::valueChanged, Slider_gamma, &QSlider::setValue);

    QObject::connect(Slider_wb, &QSlider::valueChanged, spinBox_wb, &QSpinBox::setValue);
    QObject::connect(spinBox_wb, &QSpinBox::valueChanged, Slider_wb, &QSlider::setValue);

    QObject::connect(Slider_backlight, &QSlider::valueChanged, spinBox_backlight, &QSpinBox::setValue);
    QObject::connect(spinBox_backlight, &QSpinBox::valueChanged, Slider_backlight, &QSlider::setValue);

    QObject::connect(Slider_gain, &QSlider::valueChanged, spinBox_gain, &QSpinBox::setValue);
    QObject::connect(spinBox_gain, &QSpinBox::valueChanged, Slider_gain, &QSlider::setValue);


    //===========================camera control===========================

    QObject::connect(Slider_zoom, &QSlider::valueChanged, spinBox_zoom, &QSpinBox::setValue);
    QObject::connect(spinBox_zoom, &QSpinBox::valueChanged, Slider_zoom, &QSlider::setValue);

    QObject::connect(Slider_focus, &QSlider::valueChanged, spinBox_focus, &QSpinBox::setValue);
    QObject::connect(spinBox_focus, &QSpinBox::valueChanged, Slider_focus, &QSlider::setValue);

    QObject::connect(Slider_exposure, &QSlider::valueChanged, spinBox_exposure, &QSpinBox::setValue);
    QObject::connect(spinBox_exposure, &QSpinBox::valueChanged, Slider_exposure, &QSlider::setValue);

    QObject::connect(Slider_aperture, &QSlider::valueChanged, spinBox_aperture, &QSpinBox::setValue);
    QObject::connect(spinBox_aperture, &QSpinBox::valueChanged, Slider_aperture, &QSlider::setValue);

    QObject::connect(Slider_panorama, &QSlider::valueChanged, spinBox_panorama, &QSpinBox::setValue);
    QObject::connect(spinBox_panorama, &QSpinBox::valueChanged, Slider_panorama, &QSlider::setValue);

    QObject::connect(Slider_tilt, &QSlider::valueChanged, spinBox_tilt, &QSpinBox::setValue);
    QObject::connect(spinBox_tilt, &QSpinBox::valueChanged, Slider_tilt, &QSlider::setValue);

    QObject::connect(Slider_roll, &QSlider::valueChanged, spinBox_roll, &QSpinBox::setValue);
    QObject::connect(spinBox_roll, &QSpinBox::valueChanged, Slider_roll, &QSlider::setValue);
}

void VideoCaptureFilter::onBrightnessChanged(int value) {
    emit brightnessChanged(value); // 发射信号，传递滑块的值
}

void VideoCaptureFilter::onContrastChanged(int value){
    emit contrastChanged(value);
}
void VideoCaptureFilter::onHueChanged(int value){
    emit hueChanged(value);
}
void VideoCaptureFilter::onSaturabilityChanged(int value){
    emit saturabilityChanged(value);
}
void VideoCaptureFilter::onPlainChanged(int value){
    emit plainChanged(value);
}
void VideoCaptureFilter::onGammaChanged(int value){
    emit gammaChanged(value);
}
void VideoCaptureFilter::onWbChanged(int value){
    emit wbChanged(value);
}
void VideoCaptureFilter::onBacklightChanged(int value){
    emit backlightChanged(value);
}
void VideoCaptureFilter::onGainChanged(int value){
    emit gainChanged(value);
}
void VideoCaptureFilter::onAutoWBChanged(bool value){
    emit autoWBChanged(value);
}
