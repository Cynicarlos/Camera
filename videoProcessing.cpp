#include <opencv2/opencv.hpp>
#include <math.h>


using namespace cv;
using namespace std;

// 调整亮度
Mat adjustBrightness(const Mat& img, double alpha, int beta) {
    Mat new_image = Mat::zeros(img.size(), img.type());
    // alpha 是增益（对比度），beta 是添加到每个像素的值（亮度）
    img.convertTo(new_image, -1, alpha, beta);
    return new_image;
}

// 调整色度
Mat adjustHue(const Mat& img, double delta) {
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV); // 转换为HSV色彩空间

    // 分离通道
    vector<Mat> channels;
    split(hsv, channels);

    // 调整H（色度）通道
    channels[0].convertTo(channels[0], -1, 1, delta);

    // 合并通道
    Mat new_image;
    merge(channels, hsv);

    // 转换回BGR色彩空间
    cvtColor(hsv, new_image, COLOR_HSV2BGR);
    return new_image;
}

// 调整对比度
Mat adjustContrast(const Mat& img, double alpha) {

    Mat new_image ;
    Mat gray_image;
    Mat new_gray_image;
    cvtColor(img,gray_image,COLOR_BGR2GRAY);
    img.convertTo(new_image,-1,(alpha/32.0+0.1),0);
    cvtColor(new_image,new_gray_image,COLOR_BGR2GRAY);
    double img_avg = sum(gray_image)[0] / (gray_image.rows * gray_image.cols);
    double new_img_avg = sum(new_image)[0] / (new_image.rows * new_image.cols);
    double delta = img_avg - new_img_avg;
    new_image.convertTo(new_image,-1,0,delta);
    return new_image;
}

// 调整饱和度
Mat adjustSaturation(const Mat& img, double delta) {
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV); // 转换为HSV色彩空间

    // 分离通道
    vector<Mat> channels;
    split(hsv, channels);

    // 调整S（饱和度）通道
    channels[1].convertTo(channels[1], -1, delta/64.0,0);

    // 合并通道
    Mat new_image;
    merge(channels, hsv);

    // 转换回BGR色彩空间
    cvtColor(hsv, new_image, COLOR_HSV2BGR);
    return new_image;
}

// 插值放大
Mat adjustSize(const Mat& img, double scale) {
    Size newSize(static_cast<int>(img.cols * scale), static_cast<int>(img.rows * scale));

    // 放大图像
    Mat resizedImage;
    resize(img, resizedImage, newSize, 0, 0, INTER_CUBIC); // 使用线性插值
    return resizedImage;
}

// 伽马矫正
Mat gammaCorrection(const Mat& src, double gamma) {
    Mat correctedImage;
    src.convertTo(correctedImage, CV_32F); // 转换为32位浮点数以避免溢出

    // 创建查找表
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i) {
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    }

    // 应用查找表
    LUT(src, lookUpTable, correctedImage);

    // 转换回原始图像的深度
    correctedImage.convertTo(correctedImage, src.type());

    return correctedImage;
}

// 色温调整
Mat adjustColorTemperature(const Mat& src, double temperature) {

    Mat adjustedImage;
    double temp;
    temp = temperature /100;

    double r, g, b;
    r = g = b =0;

    // 计算红色分量
    if (temp <= 66)
        r = 255;
    else{
        double tmpCalc = temp - 55;
        r = 351.976905668057 + 0.114206453784165 * tmpCalc + -40.2536630933213 * log(tmpCalc);
        if (r>255)
            r = 255;
        else if (r<0)
            r = 0;}// 限制在0-255之间

    // 计算绿色分量
    if (temp <= 66){
        double tmpCalc = temp-2;
        g = -155.254855627092 + -0.445969504695791 * tmpCalc + 104.492161993939 * log(tmpCalc);
        if (g>255)
            g = 255;
        else if (g<0)
            g = 0;} // 限制在0-255之间
    else{
        double  tmpCalc = temp - 50;
        g = 325.449412571197 + 7.94345653666234E-02 * tmpCalc + -28.0852963507957 * log(tmpCalc);
        if (g>255)
            g = 255;
        else if (g<0)
            g = 0;}  // 限制在0-255之间

    // 计算蓝色分量
    if (temp >= 66)
        b = 255;
    else if(temp <= 19)
        b = 0;
    else{
        double tmpCalc = temp - 10;
        b = -254.769351841209 + 0.827409606400739 * tmpCalc + 115.679944010661 * log(tmpCalc);
        if (b>255)
            b = 255;
        else if (b<0)
            b = 0;
    }  // 限制在0-255之间


    vector<Mat> channels;
    split(src, channels);
    channels[0].convertTo(channels[0],-1,b/255,0);
    channels[1].convertTo(channels[1],-1,g/255,0);
    channels[2].convertTo(channels[2],-1,r/255,0);
    merge(channels,adjustedImage);

    return adjustedImage;
}

// 逆光对比调整
Mat adjustBrightnessContrast(const Mat& img, double alpha, int beta) {
    Mat yuv_image;
    Mat adjusted_yuv_image;
    cvtColor(img, yuv_image, COLOR_BGR2YUV);

    // 分离YUV通道

    // 遍历所有像素点，调整亮度和对比度
    for (int y = 0; y < yuv_image.rows; y++) {
        for (int x = 0; x < yuv_image.cols; x++) {
            for (int c = 0; c < yuv_image.channels(); c++) {
                yuv_image.at<Vec3b>(y, x)[c] =
                    saturate_cast<uchar>(alpha * yuv_image.at<Vec3b>(y, x)[c] + beta);
            }
        }
    }
    cvtColor(yuv_image, adjusted_yuv_image, COLOR_YUV2BGR);

    return adjusted_yuv_image;
}


Mat grayWorld(const Mat& inputImage) {
    // 检查输入图像是否为三通道彩色图像
    if (inputImage.channels() != 3) {
        throw runtime_error("输入图像必须是三通道彩色图像");
    }

    // 初始化输出图像
    Mat outputImage = Mat::zeros(inputImage.size(), inputImage.type());

    // 计算每个通道的平均值
    Scalar meanValue = mean(inputImage);
    double R_avg = meanValue[0];
    double G_avg = meanValue[1];
    double B_avg = meanValue[2];

    // 计算几何平均值
    double grayValue = pow(R_avg * G_avg * B_avg, 1.0 / 3.0);

    // 计算增益值
    double Kr = grayValue / R_avg;
    double Kg = grayValue / G_avg;
    double Kb = grayValue / B_avg;

    // 调整每个通道
    vector<Mat> channels(3);
    split(inputImage, channels);
    channels[0] = channels[0] * Kr;
    channels[1] = channels[1] * Kg;
    channels[2] = channels[2] * Kb;

    // 合并通道
    merge(channels, outputImage);

    // 限制像素值在0到255之间
    minMaxLoc(outputImage, NULL, NULL, NULL, NULL);
    outputImage.convertTo(outputImage, CV_8U);

    return outputImage;
}

Mat whitePatch(const Mat& inputImage) {
    // 检查输入图像是否为三通道彩色图像
    if (inputImage.channels() != 3) {
        throw runtime_error("输入图像必须是三通道彩色图像");
    }

    // 初始化输出图像
    Mat outputImage = Mat::zeros(inputImage.size(), inputImage.type());

    // 找到每个通道的最大值
    double R_max, G_max, B_max;
    minMaxLoc(inputImage, NULL, NULL, NULL, NULL, Mat());
    minMaxLoc(inputImage, NULL, &R_max, NULL, NULL, Mat());
    minMaxLoc(inputImage, NULL, &G_max, NULL, NULL, Mat());
    minMaxLoc(inputImage, NULL, &B_max, NULL, NULL, Mat());

    // 计算增益值
    double Kr = 255.0 / R_max;
    double Kg = 255.0 / G_max;
    double Kb = 255.0 / B_max;

    // 调整每个通道
    vector<Mat> channels(3);
    split(inputImage, channels);
    channels[0] = channels[0] * Kr;
    channels[1] = channels[1] * Kg;
    channels[2] = channels[2] * Kb;

    // 合并通道
    merge(channels, outputImage);

    // 限制像素值在0到255之间
    minMaxLoc(outputImage, NULL, NULL, NULL, NULL);
    outputImage.convertTo(outputImage, CV_8U);

    return outputImage;
}
