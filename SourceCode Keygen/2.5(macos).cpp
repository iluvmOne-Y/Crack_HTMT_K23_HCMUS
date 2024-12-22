#include <iostream>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

using namespace std;

string LengthString(string s) {
    if (s.empty())
        return "";
    return string(1, s.back());
}

void clearScreen() {
    cout << "\033[2J\033[H";
}

void Out(char a[16][16], string s) {
    clearScreen();
    cout << "NAME :" << s << endl;
    cout << "HOW TO GET SERIAL : 9 is Snake - C is Apple - D is Destination" << endl;
    cout << "Move Snake to eat all Apple(C) and then move to Destination (D)" << endl;
    cout << "NOTE : This code isn't incomplete." << endl;
    cout << "1. Moving snake reasonably to get SERIAL correctly." << endl;
    cout << "2. Avoid moving snake circles." << endl << endl;
    cout << "HOW TO MOVE :" << endl;
    cout << "W - UP" << endl;
    cout << "S - DOWN" << endl;
    cout << "A - LEFT" << endl;
    cout << "D - RIGHT" << endl << endl;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (a[i][j] == 'C') {
                cout << "\033[32m" << a[i][j] << "\033[0m  ";
            } else if (a[i][j] == 'D') {
                cout << "\033[31m" << a[i][j] << "\033[0m  ";
            } else if (a[i][j] == '9') {
                cout << "\033[33m" << a[i][j] << "\033[0m  ";
            } else {
                cout << a[i][j] << "  ";
            }
        }
        cout << endl;
    }
}

void Init(char a[16][16]) {
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
            a[i][j] = '0';
}

void ShowKey(string &s) {
    while (s.find("0000") != string::npos)
        s.replace(s.find("0000"), 4, "C");
    while (s.find("1111") != string::npos)
        s.replace(s.find("1111"), 4, "D");
    while (s.find("2222") != string::npos)
        s.replace(s.find("2222"), 4, "E");
    while (s.find("3333") != string::npos)
        s.replace(s.find("3333"), 4, "F");
}

void ShowKey2(string &s) {
    while (s.find("000") != string::npos)
        s.replace(s.find("000"), 3, "8");
    while (s.find("111") != string::npos)
        s.replace(s.find("111"), 3, "9");
    while (s.find("222") != string::npos)
        s.replace(s.find("222"), 3, "A");
    while (s.find("333") != string::npos)
        s.replace(s.find("333"), 3, "B");
}

void ShowKey3(string &s) {
    while (s.find("00") != string::npos)
        s.replace(s.find("00"), 2, "4");
    while (s.find("11") != string::npos)
        s.replace(s.find("11"), 2, "5");
    while (s.find("22") != string::npos)
        s.replace(s.find("22"), 2, "6");
    while (s.find("33") != string::npos)
        s.replace(s.find("33"), 2, "7");
}

int EnteredName(char a[16][16], string s, int &rsour, int &csour, int &rdes, int &cdes) {
    int dl = 0;
    int length = s.length();
    int div, mod;
    int rapple[20], capple[20];
    
    for (int i = 0; i < length; i++) {
        dl += s[i];
    }

    int temp;
    for (int i = 0; i < length; i++) {
        temp = int(s[i]) ^ dl;
        dl -= temp;
        if (dl < 0) dl += 256;
        div = (temp % 256) / 16;
        mod = (temp % 256) % 16;
        if (a[div][mod] == '0')
            a[div][mod] = 'C';
        else
            return 0;
        rapple[i] = div;
        capple[i] = mod;
    }

    dl ^= temp;
    temp -= dl;
    if (temp < 0) temp += 256;
    div = (temp % 256) / 16;
    mod = (temp % 256) % 16;
    if (a[div][mod] == '0')
        a[div][mod] = 'D';
    else
        return 0;
    rdes = div;
    cdes = mod;

    temp = dl;
    div = (temp % 256) / 16;
    mod = (temp % 256) % 16;
    if (a[div][mod] == '0')
        a[div][mod] = '9';
    else
        return 0;
    rsour = div;
    csour = mod;

    return 1;
}

int kbhit(void) {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= (ICANON | ECHO);
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

int main() {
    char a[16][16];
    int rsour, csour, rdes, cdes;
    string s = " ", name;
    Init(a);

    while (true) {
        cout << "NAME :";
        getline(cin, name);
        cout << endl << endl;
        if (EnteredName(a, name, rsour, csour, rdes, cdes)) {
            Out(a, name);
            break;
        }
    }

    int k = 1;
    while (k) {
        if (kbhit()) {
            char ch = getch();
            switch (tolower(ch)) {
            case 'w':
                if (LengthString(s) == "0")
                    break;
                a[rsour][csour] = '0';
                if (rsour - 1 < 0)
                    break;
                rsour--;
                a[rsour][csour] = '9';
                s += "1";
                Out(a, name);
                break;
            case 's':
                if (LengthString(s) == "1")
                    break;
                a[rsour][csour] = '0';
                if (rsour + 1 > 15)
                    break;
                rsour++;
                a[rsour][csour] = '9';
                s += "0";
                Out(a, name);
                break;
            case 'a':
                if (LengthString(s) == "3")
                    break;
                a[rsour][csour] = '0';
                if (csour - 1 < 0)
                    break;
                csour--;
                a[rsour][csour] = '9';
                s += "2";
                Out(a, name);
                break;
            case 'd':
                if (LengthString(s) == "2")
                    break;
                a[rsour][csour] = '0';
                if (csour + 1 > 15)
                    break;
                csour++;
                a[rsour][csour] = '9';
                s += "3";
                Out(a, name);
                break;
            }
            if (csour == cdes && rsour == rdes) {
                k = 0;
            }
        }
    }

    cout << "Move :" << s << endl;
    ShowKey(s);
    ShowKey2(s);
    ShowKey3(s);
    cout << "SERIAL is :" << s << endl;
    cout << "Press Enter to exit..." << endl;
    cin.get();
    return 0;
}