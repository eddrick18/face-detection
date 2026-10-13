#pragma once
#include "core.hpp"
#include <opencv2/core.hpp>
#include <opencv2/objdetect/face.hpp>
#include <opencv2/videoio.hpp>
namespace face {
struct Detection { Box box; float confidence; };
class Detector {
    cv::Ptr<cv::FaceDetectorYN> model_;
    cv::Size input_;
public:
    explicit Detector(const Config& config);
    std::vector<Detection> detect(const cv::Mat& frame);
};
class Camera {
    cv::VideoCapture capture_;
public:
    explicit Camera(int index);
    cv::Mat next();
};
class Display {
public:
    Display();
    ~Display() noexcept;
    Display(const Display&)=delete;
    Display& operator=(const Display&)=delete;
    bool show(cv::Mat& frame,const std::vector<Detection>& faces,double fps,double latency);
};
}
