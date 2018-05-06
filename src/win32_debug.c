LONG WINAPI
nbytes__win32_exception_callback(LPEXCEPTION_POINTERS p)
{
    HANDLE Process = GetCurrentProcess();
    DWORD PID = GetProcessId(Process);
    HANDLE File = CreateFileA("crashdump.dmp", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    MINIDUMP_EXCEPTION_INFORMATION E;
    E.ThreadId = GetCurrentThreadId();
    E.ExceptionPointers = p;
    E.ClientPointers = true;
    int type = MiniDumpWithPrivateReadWriteMemory | MiniDumpWithDataSegs | MiniDumpWithHandleData |
               MiniDumpWithFullMemoryInfo | MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules;
    MiniDumpWriteDump(Process, PID, File, (MINIDUMP_TYPE)type , &E, 0, 0);
    CloseHandle(File);
    MessageBoxA(0, "nBytes fatal error\n\nMinidump file created in working dir.", "FATAL ERROR",  MB_OK | MB_ICONERROR | MB_TOPMOST);
    return EXCEPTION_EXECUTE_HANDLER;
}

bool
nbytes_init_debugging()
{
    return (SetUnhandledExceptionFilter(nbytes__win32_exception_callback) != NULL);
}