#include <Windows.h>
#include <stdio.h>

// Flag to indicate if the exception has already been handled
BOOL exceptionHandled = FALSE;

// Function to be executed after the exception
DWORD WINAPI RedirectedExecution(LPVOID lpParam)
{
    printf("\t[x] Executed after the exception\n");
    return 0;
}

LONG WINAPI CustomUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
    // Check if we have already handled the exception
    if (!exceptionHandled)
    {
        exceptionHandled = TRUE;  // Mark the exception as handled

        printf("[x] Exception code: 0x%X (divide by zero hata kodu)\n", pExceptionInfo->ExceptionRecord->ExceptionCode);

        // Create a new thread to execute the RedirectedExecution function
        HANDLE hThread = CreateThread(NULL, 0, RedirectedExecution, NULL, 0, NULL);
        if (hThread)
        {
            WaitForSingleObject(hThread, INFINITE);  //bu satır, ana fonksiyonun devam etmeden önce, oluşturulan thread'in görevini tamamlamasını sağlar.
            CloseHandle(hThread);  // Close the thread handle
        }

    /* aslında bu if/else bloğunda yapılmak istenen işlem aşağıdaki gibi EIP'i doğrudan değiştirmektir, yani RedirectedExecution fonksiyonunu çağırmak için 
        exception işleyicisinde (CustomExceptionHandler) işlemci işaretçisi (EIP) değiştirilmektedir. 
            Ancak bu tür bir doğrudan EIP değişikliği, sistemde bellek erişim hatalarına (Memory Access Violation) yol açmaktadır. 
                Bu durumu düzeltecek bir çözüm, RedirectedExecution fonksiyonunu bir thread oluşturarak çalıştırmaktır.

    // Modify the instruction pointer (EIP) to jump to the redirected execution
	    pExceptionInfo->ContextRecord->Eip = (DWORD_PTR)RedirectedExecution;

    */

        return EXCEPTION_CONTINUE_EXECUTION;
    }
    else
    {
        // If we already handled the exception, terminate the process
        return EXCEPTION_EXECUTE_HANDLER;
    }
}

void CheckDebuggerAndTriggerException()
{
    if (IsDebuggerPresent())  //bu if'in içindeki kodlar SEH mekanizmasının temel bileşenleridir. Hataların nasıl ele alınacağını belirlemek için kullanılır
    {
        __try   //Eğer bir hata oluşursa, kontrol otomatik olarak __except bloğuna geçecektir.
        {
            // Cause an exception to occur (divide by zero)
            int zero = 0;
            int result = 1 / zero;  // This will trigger a divide by zero exception
            //divide by zero örnek hatalardan biri sadece, bunun yerine bellek erişim hatası da olabilirdi.
        }
        __except (CustomUnhandledExceptionFilter(GetExceptionInformation()))    //hata işleme kodları
        {
            printf("\t[-] Divide by Zero Exception handled by the __except block.\n");
        }
    }
    else
    {
        printf("\t[+] No debugger detected. Normal execution continues.\n");
    }
}

int main()
{
    CheckDebuggerAndTriggerException();
    printf("[+] Program executed successfully.\n");
    return 0;
}
