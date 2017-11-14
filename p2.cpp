#include <iostream>
#include <fstream>
#include <ctime>
#include <afxres.h>
using namespace std;

int main() {
    HANDLE eventP = OpenEvent(EVENT_ALL_ACCESS, FALSE, "eventP");
    HANDLE eventP2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "eventP2");

    SYSTEMTIME st;
    ifstream bufferIS;
    ofstream p2OS;
    p2OS.open("buf/bufProcessP2.txt");
    ofstream logP2;
    logP2.open("log/logProcessP2.log");

    while(true) {
        WaitForSingleObject(eventP2, INFINITE );
        ResetEvent(eventP2);

        bufferIS.open("buf/buf.txt");
        int num;
        bufferIS>>num;
        bufferIS.close();

        char b[100];
        GetLocalTime(&st);
        sprintf(b, "%02d:%02d:%02d.%03d",
                st.wHour,
                st.wMinute,
                st.wSecond,
                st.wMilliseconds );
        logP2<<b<<" INFO processP2 - read "<<num<<" from buffer.txt"<<endl;

        p2OS<<num<<endl;
        GetLocalTime(&st);
        sprintf(b, "%02d:%02d:%02d.%03d",
                st.wHour,
                st.wMinute,
                st.wSecond,
                st.wMilliseconds );
        logP2<<b<<" INFO processP2 - write "<<num<<" to p2.txt"<<endl;

        if (num == -1) {
            GetLocalTime(&st);
            sprintf(b, "%02d:%02d:%02d.%03d",
                    st.wHour,
                    st.wMinute,
                    st.wSecond,
                    st.wMilliseconds );
            logP2<<b<<" INFO processP2 - closed";
            logP2.close();
            p2OS.close();
            SetEvent(eventP2);
            break;
        }
        SetEvent(eventP);
    }
    return 0;
}