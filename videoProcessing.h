#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 调整亮度
Mat adjustBrightness(const Mat& img, double alpha, int beta);
// 调整色度
Mat adjustHue(const Mat& img, double delta);

// 调整对比度
Mat adjustContrast(const Mat& img, double delta);

// 调整饱和度
Mat adjustSaturation(const Mat& img, double alpha) ;

// 插值放大
Mat adjustSize(const Mat& img, double scale);
// 伽马矫正
Mat gammaCorrection(const Mat& src, double gamma) ;

// 色温调整
Mat adjustColorTemperature(const Mat& src, double temperature);
// 逆光对比调整
Mat adjustBrightnessContrast(const Mat& img, double alpha, int beta);
// 自动白平衡
Mat grayWorld(const Mat& img);
Mat whitePatch(const Mat& img);
