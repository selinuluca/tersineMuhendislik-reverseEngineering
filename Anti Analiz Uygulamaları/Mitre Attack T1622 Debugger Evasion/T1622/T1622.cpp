#include <iostream>
#include <windows.h>

#pragma comment(lib, "ntdll.lib") 

#define NtCurrentProcess() (HANDLE)-1 

extern "C" NTSTATUS NTAPI NtQueryInformationProcess(HANDLE hProcess, ULONG InfoClass, PVOID Buffer, ULONG Length, PULONG ReturnLength); //Debug Portu sorgulayabilmek için

#ifdef _WIN64
#define readpeb (PBOOLEAN)__readgsqword(0x60) + 2//PEB (Process Environment Block) yapısında BeingDebugged flag'ıne işaret ediyor. x64 için
#else
#define readpeb (PBOOLEAN)__readfsdword(0x30) + 2 //x32 için
#endif

class DebuggerDetector {
public:
    static bool DetectDebugger(bool &flag) {
        PBOOLEAN BeingDebugged = readpeb;
        HANDLE DebugPort = NULL; //sıfırsa debugger yoktur

        if (IsDebuggerPresent()) {
            std::cout << "Debugger Detected via IsDebuggerPresent API!!!" << std::endl;
            flag = true;
        }
        else
            std::cout << "No debugger detected via IsDebuggerPresent API!" << std::endl;

        if (NtQueryInformationProcess(NtCurrentProcess(), 7, &DebugPort, sizeof(HANDLE), NULL) == 0) {  // 7 = debugPort
            if (DebugPort) {
                std::cout << "Debugger Detected via QueryInformationProcess!!!" << std::endl;
                flag = true;
            }

            else
                std::cout << "No debugger detected via QueryInformationProcess" << std::endl;
        }

        if (*BeingDebugged) {
            std::cout << "Debugger Detected via PEB BeingDebugged Flag!!!" << std::endl;
            flag = true;
        }

        else
            std::cout << "No debugger detected via PEB BeingDebugged Flag!" << std::endl;

        return flag;
    }
};

int main() {
    bool flag = false;
    if(DebuggerDetector::DetectDebugger(flag))
        std::cout << "\nThere is debugger" << std::endl;
    else
        std::cout << "\nHello, World!" << std::endl;
    return 0;
}
