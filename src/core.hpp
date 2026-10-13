#pragma once
#include <filesystem>
#include <string>
#include <vector>
namespace face {
struct Config {
    int camera = 0, width = 320, height = 320;
    float threshold = 0.8F;
    std::filesystem::path model = "models/face_detection_yunet_2023mar.onnx";
    bool help = false, check_model = false;
};
struct Box { int x, y, width, height; };
Config parse(const std::vector<std::string>& args);
Box map_box(double x, double y, double w, double h, int iw, int ih, int fw, int fh);
std::vector<unsigned char> read_model(const std::filesystem::path& path);
const char* usage();
}
