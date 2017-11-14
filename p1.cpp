#include <iostream>
#include <fstream>
#include <ctime>
#include <afxres.h>
using namespace std;

int main() {
    HANDLE eventP1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "eventP1");
    HANDLE eventP2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "eventP2");

    SYSTEMTIME st;
    ifstream bufferIS;
    ofstream logP1;
    logP1.open("log/logProcessP1.log");

    while(true) {
        WaitForSingleObject(eventP1, INFINITE );
        ResetEvent(eventP1);

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
        logP1<<b<<" INFO processP1 - read "<<num<<" from buffer.txt"<<endl;

        cout<<num<<endl;
        GetLocalTime(&st);
        sprintf(b, "%02d:%02d:%02d.%03d",
                st.wHour,
                st.wMinute,
                st.wSecond,
                st.wMilliseconds );
        logP1<<b<<" INFO processP1 - write "<<num<<" to console"<<endl;

        if (num == -1) {
            GetLocalTime(&st);
            sprintf(b, "%02d:%02d:%02d.%03d",
                    st.wHour,
                    st.wMinute,
                    st.wSecond,
                    st.wMilliseconds );
            logP1<<b<<" INFO processP1 - closed";
            logP1.close();
            SetEvent(eventP2);
            break;
        }
        SetEvent(eventP2);
    }
    return 0;
}