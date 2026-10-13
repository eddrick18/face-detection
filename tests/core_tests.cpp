#include "core.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <limits>
void check(bool value) { if(!value) throw std::runtime_error("Test assertion failed"); }
template<class F> void rejects(F f) { bool rejected=false; try { f(); } catch(const std::exception&) { rejected=true; } check(rejected); }
int main() {
 try {
    check(face::parse({}).camera==0);
    check(face::parse({"--camera","255","--threshold","1","--width","2048"}).width==2048);
    for(const auto& value:{"0","nan","inf","1.1","0.8junk"}) rejects([&]{face::parse({"--threshold",value});});
    for(const auto& value:{"-1","256","3x","99999999999999999"}) rejects([&]{face::parse({"--camera",value});});
    rejects([]{face::parse({"--model"});}); rejects([]{face::parse({"--model",""});});
    rejects([]{face::parse({"--bogus","0"});}); rejects([]{face::parse({"--width","33"});});
    const auto b=face::map_box(-10,10,50,50,100,100,200,300);
    check(b.x==0 && b.y==30 && b.width==80 && b.height==150);
    check(face::map_box(200,200,10,10,100,100,200,300).width==0);
    check(face::map_box(0,0,-1,10,100,100,100,100).width==0);
    check(face::map_box(std::numeric_limits<double>::quiet_NaN(),0,10,10,100,100,100,100).width==0);
    rejects([]{face::map_box(0,0,10,10,0,100,100,100);});
    const auto path=std::filesystem::temp_directory_path()/std::filesystem::path(u8"face-test-\u30e2\u30c7\u30eb.bin");
    struct Cleanup { std::filesystem::path p; ~Cleanup(){std::error_code ec;std::filesystem::remove(p,ec);} } cleanup{path};
    { std::ofstream file(path,std::ios::binary); file<<"abc"; }
    check(face::read_model(path).size()==3);
    { std::ofstream file(path,std::ios::binary|std::ios::trunc); }
    rejects([&]{face::read_model(path);});
    std::filesystem::remove(path); rejects([&]{face::read_model(path);});
    std::cout<<"Configuration, geometry, Unicode model-path and file-failure checks passed\n";
    return 0;
 } catch(const std::exception& e) { std::cerr<<e.what()<<'\n'; return 1; }
}
