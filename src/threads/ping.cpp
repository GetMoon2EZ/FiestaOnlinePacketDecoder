#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>

#include <stdio.h>

#include <tins/tins.h>
#include <windows.h>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_data.h"

#ifndef PING_ERROR
#define PING_ERROR  9999
#endif

#ifndef BUFSIZE
#define BUFSIZE     4096
#endif

using namespace std;

static HANDLE m_hChildStd_OUT_Rd = NULL;
static HANDLE m_hChildStd_OUT_Wr = NULL;
static HANDLE m_hreadDataFromExtProgram = NULL;
static uint32_t last_ping = PING_ERROR;

static const uint32_t get_ping_ms(const string& server_addr);
static HRESULT RunExternalProgram(const string& externalProgram, const string& arguments);
static DWORD __stdcall readDataFromPingProgram(void *argh);
static void parsePingOutput(const CHAR *buffer);

void ping_thread(uint32_t update_delta_ms)
{
    FOPDData *data = FOPDData::getInstance();

    while (true) {
        // Run the ping command as an external program
        // This should update last_ping to the ping output
        RunExternalProgram("ping", FIESTA_ONLINE_SERVER_ADDRESS + " -n 1");
        cout << "[DEBUG] ping: " << static_cast<int>(last_ping) << " ms" << endl;
        data->setPing(last_ping);
        // Pause current thread
        this_thread::sleep_for(chrono::milliseconds(update_delta_ms));
    }
}

static const uint32_t get_ping_ms(const string& server_addr)
{
    // char buffer[1024] = { 0 };
    // char cmd[256] = { 0 };
    uint32_t ping = PING_ERROR;

    // Run the ping command as an external program
    if (RunExternalProgram("ping", server_addr + " -n 1") != S_OK) {
        cerr << "[ERROR] Unable to run external program" << endl;
        return PING_ERROR; 
    }

    return ping;
}

static HRESULT RunExternalProgram(const string& externalProgram, const string& arguments)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES saAttr;

    // Set the security attributes correctly
    ZeroMemory(&saAttr, sizeof(saAttr));
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT. 

    if (!CreatePipe(&m_hChildStd_OUT_Rd, &m_hChildStd_OUT_Wr, &saAttr, 0)) {
        // log error
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // Ensure the read handle to the pipe for STDOUT is not inherited.

    if (!SetHandleInformation(m_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
        // log error
        return HRESULT_FROM_WIN32(GetLastError());
    }

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = m_hChildStd_OUT_Wr;
    si.hStdOutput = m_hChildStd_OUT_Wr;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    ZeroMemory(&pi, sizeof(pi));

    string commandLine = externalProgram + " " + arguments;

    // Start the child process. 
    if (!CreateProcessA(NULL,           // No module name (use command line)
        (TCHAR*)commandLine.c_str(),    // Command line
        NULL,                           // Process handle not inheritable
        NULL,                           // Thread handle not inheritable
        TRUE,                           // Set handle inheritance
        SW_HIDE,                        // Hide window
        NULL,                           // Use parent's environment block
        NULL,                           // Use parent's starting directory 
        &si,                            // Pointer to STARTUPINFO structure
        &pi)                            // Pointer to PROCESS_INFORMATION structure
        ) {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    m_hreadDataFromExtProgram  = CreateThread(0, 0, readDataFromPingProgram, NULL, 0, NULL);
    return S_OK;
}

static DWORD __stdcall readDataFromPingProgram(void *argh)
{
    DWORD dwRead = 0;
    CHAR chBuf[BUFSIZE] = { 0 };
    BOOL bSuccess = FALSE;

    for (;;) {
        bSuccess = ReadFile(m_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0)
            continue;

        // Parse ping output
        parsePingOutput(chBuf);

        // Reset buffer
        ZeroMemory(chBuf, dwRead);
        if (!bSuccess)
            break;
    }
    // CloseHandle(m_hChildStd_OUT_Rd);
    // CloseHandle(m_hChildStd_OUT_Wr);
    return 0;
}

static void parsePingOutput(const CHAR *buffer)
{
    const char *p = strrchr(buffer, ' ');
    if (p == nullptr) {
        cerr << "[ERROR] ping: strrchr returned NULL" << endl;
        last_ping = PING_ERROR;
        return;
    }

    if (*(p - 1) != '=') {
        cout << "[ERROR] buffer: " << buffer << endl;
        cerr << "[ERROR] ping: unreachable host" << endl;
        last_ping = PING_ERROR;
        return;
    }

    // Convert the value to an int
    uint32_t ping = static_cast<uint32_t>(strtol(p, NULL, 10));

    // strtol returns 0 if no valid value in string and ping cannot be 0
    last_ping = ping>0?ping:PING_ERROR;
}
