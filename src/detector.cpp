#include "app.hpp"
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <stdexcept>
namespace face {
Detector::Detector(const Config& c):input_(c.width,c.height) {
    const auto bytes=read_model(c.model);
    model_=cv::FaceDetectorYN::create("onnx",bytes,{},input_,c.threshold,0.3F,5000);
    if(model_.empty()) throw std::runtime_error("Detector initialization failed");
}
std::vector<Detection> Detector::detect(const cv::Mat& frame) {
    if(frame.empty() || frame.type()!=CV_8UC3) throw std::runtime_error("Expected an 8-bit BGR camera frame");
    cv::Mat resized,output;
    cv::resize(frame,resized,input_);
    model_->detect(resized,output);
    if(!output.empty() && (output.type()!=CV_32F || output.cols!=15))
        throw std::runtime_error("Unexpected detector output");
    std::vector<Detection> results;
    for(int i=0;i<output.rows;++i) {
        const auto* r=output.ptr<float>(i);
        if(!std::isfinite(r[14]) || r[14]<0 || r[14]>1) continue;
        auto b=map_box(r[0],r[1],r[2],r[3],input_.width,input_.height,frame.cols,frame.rows);
        if(b.width>0 && b.height>0) results.push_back({b,r[14]});
    }
    return results;
}
}
