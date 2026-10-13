#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace filesystem;

int main(){
    if(!exists("bin")){
        cout << "Chưa có folder bin, đang tạo..." << endl;
        create_directories("bin");
        cout << "Đã tạo folder bin" << endl;
    }
    cout << "Đang biên dịch vui lòng chờ ...";
    system("g++ src\\main.cpp src\\lib\\qrcodegen.cpp -o bin\\main.exe");
    system("bin\\main.exe");
    return 0;
}