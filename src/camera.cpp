#include "app.hpp"
#include <stdexcept>
namespace face {
Camera::Camera(int index):capture_(index,cv::CAP_ANY) {
    if(!capture_.isOpened()) throw std::runtime_error("Cannot open webcam: check index, permissions, and other camera apps");
}
cv::Mat Camera::next() {
    cv::Mat frame;
    if(!capture_.read(frame) || frame.empty()) throw std::runtime_error("Camera disconnected or returned an unreadable frame");
    return frame;
}
}
