#include "core.hpp"
#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>
#include <stdexcept>
namespace face {
namespace {
int integer(const std::string& s, int lo, int hi) {
    int v{}; auto [end, ec] = std::from_chars(s.data(), s.data()+s.size(), v);
    if(ec != std::errc{} || end != s.data()+s.size() || v < lo || v > hi)
        throw std::invalid_argument("Invalid integer: " + s);
    return v;
}
}
Config parse(const std::vector<std::string>& args) {
    Config c;
    for(std::size_t i=0; i<args.size(); ++i) {
        const auto& key=args[i];
        if(key=="--help") { c.help=true; continue; }
        if(key=="--check-model") { c.check_model=true; continue; }
        if(key!="--camera" && key!="--model" && key!="--threshold" && key!="--width" && key!="--height")
            throw std::invalid_argument("Unknown option: " + key);
        if(++i==args.size()) throw std::invalid_argument("Missing value for " + key);
        const auto& value=args[i];
        if(key=="--camera") c.camera=integer(value,0,255);
        else if(key=="--width") c.width=integer(value,32,2048);
        else if(key=="--height") c.height=integer(value,32,2048);
        else if(key=="--model") {
            if(value.empty()) throw std::invalid_argument("Empty model path");
            c.model=std::filesystem::path(std::u8string(value.begin(), value.end()));
        } else {
            float v{}; auto [end,ec]=std::from_chars(value.data(),value.data()+value.size(),v);
            if(ec!=std::errc{} || end!=value.data()+value.size() || !std::isfinite(v) || v<=0 || v>1)
                throw std::invalid_argument("Threshold must be in (0, 1]");
            c.threshold=v;
        }
    }
    if(c.width%32 || c.height%32) throw std::invalid_argument("Input dimensions must be multiples of 32");
    return c;
}
Box map_box(double x,double y,double w,double h,int iw,int ih,int fw,int fh) {
    if(iw<=0 || ih<=0 || fw<=0 || fh<=0) throw std::invalid_argument("Invalid frame dimensions");
    if(!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(w)||!std::isfinite(h)||w<=0||h<=0)
        return {0,0,0,0};
    const double sx=static_cast<double>(fw)/iw, sy=static_cast<double>(fh)/ih;
    const int left=static_cast<int>(std::floor(std::clamp(x*sx,0.0,static_cast<double>(fw))));
    const int top=static_cast<int>(std::floor(std::clamp(y*sy,0.0,static_cast<double>(fh))));
    const int right=static_cast<int>(std::ceil(std::clamp((x+w)*sx,0.0,static_cast<double>(fw))));
    const int bottom=static_cast<int>(std::ceil(std::clamp((y+h)*sy,0.0,static_cast<double>(fh))));
    return {left,top,std::max(0,right-left),std::max(0,bottom-top)};
}
std::vector<unsigned char> read_model(const std::filesystem::path& path) {
    std::ifstream stream(path,std::ios::binary|std::ios::ate);
    if(!stream) throw std::runtime_error("Cannot open model; supply --model or run scripts/get-model.ps1");
    const auto size=stream.tellg();
    if(size<=0 || size>16*1024*1024) throw std::runtime_error("Model is empty or exceeds 16 MiB");
    std::vector<unsigned char> bytes(static_cast<std::size_t>(size));
    stream.seekg(0);
    if(!stream.read(reinterpret_cast<char*>(bytes.data()),static_cast<std::streamsize>(size)))
        throw std::runtime_error("Cannot read complete model");
    return bytes;
}
const char* usage() { return "face_detection [--camera 0] [--model PATH] [--threshold 0.8]\n"
    "               [--width 320] [--height 320] [--check-model] [--help]\n"
    "Input dimensions: multiples of 32 in [32,2048]. Quit: Q or Escape.\n"; }
}
