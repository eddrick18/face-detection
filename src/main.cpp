#include "app.hpp"
#include <chrono>
#include <iostream>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#endif
namespace {
std::vector<std::string> arguments(int argc,char** argv) {
    std::vector<std::string> result;
#ifdef _WIN32
    (void)argc; (void)argv;
    int count=0;
    auto* wide=CommandLineToArgvW(GetCommandLineW(),&count);
    if(!wide) throw std::runtime_error("Cannot read command line");
    struct Guard { wchar_t** value; ~Guard(){ LocalFree(value); } } guard{wide};
    for(int i=1;i<count;++i) {
        const int size=WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,wide[i],-1,nullptr,0,nullptr,nullptr);
        if(size<=0) throw std::runtime_error("Invalid Unicode argument");
        std::string value(static_cast<std::size_t>(size),'\0');
        if(!WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,wide[i],-1,value.data(),size,nullptr,nullptr))
            throw std::runtime_error("Cannot convert argument");
        value.pop_back(); result.push_back(value);
    }
#else
    for(int i=1;i<argc;++i) result.emplace_back(argv[i]);
#endif
    return result;
}
}
int main(int argc,char** argv) {
    try {
        const auto config=face::parse(arguments(argc,argv));
        if(config.help) { std::cout<<face::usage(); return 0; }
        face::Detector detector(config);
        if(config.check_model) {
            detector.detect(cv::Mat::zeros(config.height,config.width,CV_8UC3));
            std::cout<<"Model loaded; synthetic-frame inference passed.\n"; return 0;
        }
        std::cout<<"Opening webcam. Local processing only; no recording. Q/Escape quits.\n";
        face::Camera camera(config.camera);
        face::Display display;
        using Clock=std::chrono::steady_clock;
        auto previous=Clock::now(); double fps=0;
        while(true) {
            auto frame=camera.next();
            const auto start=Clock::now();
            auto detections=detector.detect(frame);
            const auto end=Clock::now();
            const double elapsed=std::chrono::duration<double>(end-previous).count();
            previous=end;
            if(elapsed>0) fps=fps==0 ? 1/elapsed : 0.9*fps+0.1/elapsed;
            const double latency=std::chrono::duration<double,std::milli>(end-start).count();
            if(!display.show(frame,detections,fps,latency)) break;
        }
        return 0;
    } catch(const std::exception& error) {
        std::cerr<<"Error: "<<error.what()<<'\n'; return 1;
    } catch(...) { std::cerr<<"Unexpected application failure\n"; return 1; }
}
