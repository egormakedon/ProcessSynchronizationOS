#include <iostream>
#include <fstream>
#include <afxres.h>
#include <csignal>
#include <unistd.h>
using namespace std;

SYSTEMTIME st;
HANDLE eventP;
HANDLE eventP1;
HANDLE eventP2;

ofstream bufferOS;
ofstream logOS;

void signal_handler(int signal) {
    sleep(1);

    if (bufferOS.is_open()) {
        bufferOS.close();
    }

    bufferOS.open("buf/buf.txt");
    bufferOS<<"-1"<<endl;
    bufferOS.close();

    char b[100];
    GetLocalTime(&st);
    sprintf(b, "%02d:%02d:%02d.%03d",
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds );

    logOS<<b<<" INFO processP - write -1 to buffer.txt"<<endl;
    logOS<<b<<" INFO processP - closed"<<endl;

    if (logOS.is_open()) {
        logOS.close();
    }

    SetEvent(eventP1);
    exit(0);
}

int main() {
    eventP = CreateEvent(NULL, FALSE, TRUE, "eventP");
    eventP1 = CreateEvent(NULL, FALSE, FALSE, "eventP1");
    eventP2 = CreateEvent(NULL, FALSE, FALSE, "eventP2");

    STARTUPINFO startUpInfo;
    ZeroMemory(&startUpInfo, sizeof(STARTUPINFO));
    PROCESS_INFORMATION processInformation[2];
    CreateProcess("p1.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startUpInfo, &processInformation[0]);
    CreateProcess("p2.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startUpInfo, &processInformation[1]);

    signal(SIGINT, signal_handler);
    srand(NULL);

    logOS.open("log/logProcessP.log");

    while(true) {
        WaitForSingleObject(eventP, INFINITE );
        ResetEvent(eventP);

        sleep(1);

        int num = rand() % 2000;
        bufferOS.open("buf/buf.txt");
        bufferOS<<num<<endl;
        bufferOS.close();

        char b[100];
        GetLocalTime(&st);
        sprintf(b, "%02d:%02d:%02d.%03d",
                st.wHour,
                st.wMinute,
                st.wSecond,
                st.wMilliseconds );
        logOS<<b<<" INFO processP - write "<<num<<" to buffer.txt"<<endl;

        SetEvent(eventP1);
    }
}