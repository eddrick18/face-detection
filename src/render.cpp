#include "app.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <iomanip>
#include <sstream>
namespace face {
namespace { constexpr const char* title="Face detection - camera active"; }
Display::Display() { cv::namedWindow(title,cv::WINDOW_NORMAL); }
Display::~Display() noexcept { try { cv::destroyWindow(title); } catch(...) {} }
bool Display::show(cv::Mat& frame,const std::vector<Detection>& faces,double fps,double latency) {
    for(const auto& face:faces) {
        const auto& b=face.box;
        cv::rectangle(frame,cv::Rect(b.x,b.y,b.width,b.height),cv::Scalar(0,255,0),2);
        std::ostringstream text; text<<std::fixed<<std::setprecision(2)<<face.confidence;
        cv::putText(frame,text.str(),cv::Point(b.x,std::max(15,b.y-5)),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,255,0),1);
    }
    std::ostringstream status;
    status<<"Faces: "<<faces.size()<<" | FPS: "<<std::fixed<<std::setprecision(1)<<fps
          <<" | Detect: "<<latency<<" ms | Q/Esc: quit";
    cv::putText(frame,status.str(),cv::Point(10,25),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,255,255),1);
    cv::imshow(title,frame);
    const int key=cv::waitKey(1)&255;
    return key!=27 && key!='q' && key!='Q' && cv::getWindowProperty(title,cv::WND_PROP_VISIBLE)>=1;
}
}
