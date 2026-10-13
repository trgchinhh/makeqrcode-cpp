#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <conio.h>
#include "lib/qrcodegen.hpp"

using namespace std;
using namespace filesystem;
using namespace qrcodegen;
using qrcodegen::QrCode;

const string pathfile = "qr.txt";

void print(vector<string>&menu){
    for(int i = 0; i < menu.size(); i++){
        cout << menu[i] << endl;
    }
    cout << endl;
}

void print(string context){
    cout << " [+] Nội dung: ";
    if(context.empty()) cout << "Chưa có";
    else cout << context;
    cout << "\n\n";
}

void pause(){
    cout << "\n [-] Nhấn phím bất kỳ để tiếp tục ...";
    getch();
}

bool stringtoint(string &str, int& in){
    try {
        size_t pos; in = stoi(str, &pos);
        return pos == str.size();
    } catch(...){
        return false;
    }
}

void writefile_func(string context = "", string qrcode = "", string pathfile = "qr.txt"){
    vector<string> menu = {
        " \t[01] Có",
        " \t[02] Không"
    };
    if(exists(pathfile)){
        cout << " \t[!] File: " << pathfile << " đã tồn tại, bạn có muốn ghi đè !" << endl;
        print(menu);
        string menuchoice; int menuchoice_int;
        cout << " \t[?] Chọn: "; getline(cin, menuchoice);
        if(!stringtoint(menuchoice, menuchoice_int)){
            cout << " \t[!] Vui lòng nhập số nguyên !!!" << endl;
            return;
        }
        if(menuchoice_int == 1){
            ofstream file(pathfile);
            if(!file.is_open()){
                cout << " \t[!] Không mở được file: " << pathfile << endl;
                return;
            }
            file << "Nội dung Qrcode: " << context << "\n\n"; 
            file << qrcode;
            file.close();
            cout << " \t[*] Đã ghi vào qr vào file: " << pathfile << endl;
        }
        else return;
    }
}

string createqrcode(string context){
    QrCode qr = QrCode::encodeText(
        context.c_str(), 
        QrCode::Ecc::LOW
    );
    stringstream os;
    int size = qr.getSize();
    int border = 1;
    int width = size + border * 2;   
    os << "\t┌";
    for (int i = 0; i < width; i++)
        os << "─";
    os << "┐" << endl;
    for (int y = -border; y < size + border - 1; y += 2) {
        os << "\t│";
        for (int x = -border; x < size + border; x++) {
            bool top = qr.getModule(x, y);
            bool bot = qr.getModule(x, y + 1);
            if (top && bot) os << "█";
            else if (top)   os << "▀";
            else if (bot)   os << "▄";
            else            os << " ";
        }
        os << "│" << endl;
    }
    os << "\t└";
    for (int i = 0; i < width; i++)
        os << "─";
    os << "┘" << endl;
    return os.str();
}

bool checkcontextisnull(string context){
    if(context.empty()) return true;
    else return false;
}

void printqrcode(string context){
    cout << " Qr code - nội dung: " << context << endl;
    cout << createqrcode(context) << endl;
}

int main(){
    vector<string> banner = {
        " ╔═════════════════════════════════╗",
        " ║           Make QRCode           ║",
        " ║ Simple C++ Make QRcode Program  ║",
        " ║ Author: Truong Chinh            ║",
        " ║ Github: Github.com/trgchinhh    ║",
        " ╚═════════════════════════════════╝",
    };
    string context = "";
    vector<string> menu = {
        " [01] Nhập nội dung",
        " [02] Tạo Qrcode",
        " [03] Ghi Qrcode vào file",
        " [04] Thoát"
    }; 
    while(true){
        system("cls");
        print(banner);
        print(context);
        print(menu);
        cout << " [?] Chọn: ";
        string menuchoice; getline(cin, menuchoice);

        int menuchoice_int;
        if(!stringtoint(menuchoice, menuchoice_int)){
            cout << " [!] Vui lòng nhập số nguyên !!!" << endl;
            pause();
            continue;
        }

        if(menuchoice_int == 1){
            cout << " \t[?] Nhập nội dung: "; 
            string data; getline(cin, data);
            if(data.empty()){
                cout << " \t[!] Vui lòng nhập lại nội dung !!!";
                continue;
            } 
            context = data;
            cout << " \t[-] Nội dung mới: " << context << endl;
            pause();
        }
        else if(menuchoice_int == 2){
            if(checkcontextisnull(context)){
                cout << " [!] Chưa có nội dung, vui lòng nhập nội dung !" << endl;
                pause();
                continue;
            }
            printqrcode(context);
            pause();
        }
        else if(menuchoice_int == 3){
            if(checkcontextisnull(context)){
                cout << " [!] Chưa có nội dung, vui lòng nhập nội dung !" << endl;
                pause();
                continue;
            }
            printqrcode(context);
            cout << " \t[?] Nhập tên file (enter để dùng tên mặc định): ";
            string pathfile; getline(cin, pathfile);
            string context_qrcode = createqrcode(context);
            if(pathfile.empty()){
                writefile_func(context, context_qrcode);
                pause();
            } 
            else{
                writefile_func(context, context_qrcode, pathfile);
                pause();                
            } 
        }
        else if(menuchoice_int == 4){
            cout << " Tạm biệt !" << endl;
            exit(0);
        } 
        else {
            cout << " [!] Vui lòng chọn đúng !!!" << endl;
            pause();
        }
    }

    return 0;
}