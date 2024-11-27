#ifndef VIDEOCAPTUREFILTER_H
#define VIDEOCAPTUREFILTER_H

#include <QDialog>
#include "ui_videocapturefilter.h" // 确保这个头文件被正确包含

class VideoCaptureFilter : public QDialog
{
    Q_OBJECT

public:
    explicit VideoCaptureFilter(QWidget *parent = nullptr);
    ~VideoCaptureFilter();

signals:
    void brightnessChanged(int value);
    void contrastChanged(int value);
    void hueChanged(int value);
    void saturabilityChanged(int value);
    void plainChanged(int value);
    void gammaChanged(int value);
    void wbChanged(int value);
    void backlightChanged(int value);
    void gainChanged(int value);

private slots:
    void init_Widget();
    void set_General_Slots();
    void bind_Slider_Spinbox();
    void Set_Amp_Default_Values();
    void Set_Camera_Control_Default_Values();
    void on_Amp_Yes();
    void on_Amp_Cannel();
    void on_Camera_Control_Yes();
    void on_Camera_Control_Cannel();

    void onBrightnessChanged(int value); // 槽函数，响应滑块值的变化
    void onContrastChanged(int value);
    void onHueChanged(int value);
    void onSaturabilityChanged(int value);
    void onPlainChanged(int value);
    void onGammaChanged(int value);
    void onWbChanged(int value);
    void onBacklightChanged(int value);
    void onGainChanged(int value);


private:
    Ui::VideoCaptureFilter *ui; // 使用Ui::VideoCaptureFilter作为前向声明

    QPushButton * pushButton_amp_default;
    QPushButton * pushButton_amp_yes;
    QPushButton * pushButton_amp_cancel;
    QPushButton * pushButton_camera_control_default;
    QPushButton * pushButton_camera_control_yes;
    QPushButton * pushButton_camera_control_cancel;


    QSlider *Slider_brightness;
    QSpinBox *spinBox_brightness;
    QCheckBox *checkBox_auto_brightness;

    QSlider *Slider_contrast;
    QSpinBox *spinBox_contrast;
    QCheckBox *checkBox_auto_contrast;

    QSlider *Slider_hue;
    QSpinBox *spinBox_hue;
    QCheckBox *checkBox_auto_hue;

    QSlider *Slider_saturability;
    QSpinBox *spinBox_saturability;
    QCheckBox *checkBox_auto_saturability;

    QSlider *Slider_plain;
    QSpinBox *spinBox_plain;
    QCheckBox *checkBox_auto_plain;

    QSlider *Slider_gamma;
    QSpinBox *spinBox_gamma;
    QCheckBox *checkBox_auto_gamma;

    QSlider *Slider_wb;
    QSpinBox *spinBox_wb;
    QCheckBox *checkBox_auto_wb;

    QSlider *Slider_backlight;
    QSpinBox *spinBox_backlight;
    QCheckBox *checkBox_auto_backlight;

    QSlider *Slider_gain;
    QSpinBox *spinBox_gain;
    QCheckBox *checkBox_auto_gain;

    //===========================camera control===========================
    QSlider *Slider_zoom;
    QSpinBox *spinBox_zoom;
    QCheckBox *checkBox_auto_zoom;

    QSlider *Slider_focus;
    QSpinBox *spinBox_focus;
    QCheckBox *checkBox_auto_focus;

    QSlider *Slider_exposure;
    QSpinBox *spinBox_exposure;
    QCheckBox *checkBox_auto_exposure;

    QSlider *Slider_aperture;
    QSpinBox *spinBox_aperture;
    QCheckBox *checkBox_auto_aperture;

    QSlider *Slider_panorama;
    QSpinBox *spinBox_panorama;
    QCheckBox *checkBox_auto_panorama;

    QSlider *Slider_tilt;
    QSpinBox *spinBox_tilt;
    QCheckBox *checkBox_auto_tilt;

    QSlider *Slider_roll;
    QSpinBox *spinBox_roll;
    QCheckBox *checkBox_auto_roll;

};

#endif // VIDEOCAPTUREFILTER_H
