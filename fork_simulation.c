/*
 * Practical: Process Creation and Management (Fork Simulation)
 * Description:
 *     Demonstrates process creation and process identification (PID and Parent PID),
 *     along with process address space isolation (memory in child is distinct from parent).
 *     - On POSIX/UNIX/Linux/macOS: uses native fork(), getpid(), getppid(), waitpid().
 *     - On Windows: uses CreateProcess() to spawn a child process with isolated memory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shared_variable = 100;

#ifdef _WIN32
#include <windows.h>

static void run_windows_process_demo(const char *exe_path) {
    printf("Note: Windows does not implement POSIX fork().\n");
    printf("Using Win32 CreateProcess() for OS-level process creation.\n");
    printf("\n--- Running Win32 Process Creation ---\n");
    printf("[PARENT PROCESS]\n");
    printf("  Parent PID: %lu\n", (unsigned long)GetCurrentProcessId());
    printf("  Initial shared_variable: %d\n", shared_variable);

    char cmd[MAX_PATH + 32];
    snprintf(cmd, sizeof(cmd), "\"%s\" --child %lu", exe_path, (unsigned long)GetCurrentProcessId());

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "CreateProcess failed (error %lu).\n", (unsigned long)GetLastError());
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("\n[BACK TO PARENT PROCESS]\n");
    printf("  Parent PID: %lu\n", (unsigned long)GetCurrentProcessId());
    printf("  shared_variable in Parent remains: %d\n", shared_variable);
    printf("  --> Proves process address spaces are isolated (independent memory copies)!\n\n");
}
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void run_posix_fork_demo(void) {
    printf("\n--- Running Native fork() (POSIX Systems) ---\n");
    printf("[PARENT PROCESS]\n");
    printf("  Parent PID: %d\n", (int)getpid());
    printf("  Initial shared_variable: %d\n", shared_variable);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        /* Child process */
        printf("\n[CHILD PROCESS]\n");
        printf("  Child Process PID  : %d\n", (int)getpid());
        printf("  Parent Process PID : %d\n", (int)getppid());
        shared_variable += 50;
        printf("  Child modified shared_variable to: %d\n", shared_variable);
        usleep(500000);
        printf("  Child process exiting.\n");
        exit(0);
    } else {
        /* Parent process */
        printf("\n[PARENT]\n");
        printf("  My PID    = %d\n", (int)getpid());
        printf("  Child PID = %d\n", (int)pid);
        waitpid(pid, NULL, 0);

        printf("\n[BACK TO PARENT PROCESS]\n");
        printf("  Parent PID: %d\n", (int)getpid());
        printf("  shared_variable in Parent remains: %d\n", shared_variable);
        printf("  --> Proves process address spaces are isolated (independent memory copies)!\n\n");
    }
}
#endif

int main(int argc, char *argv[]) {
#ifdef _WIN32
    if (argc >= 3 && strcmp(argv[1], "--child") == 0) {
        printf("\n[CHILD PROCESS]\n");
        printf("  Child Process PID  : %lu\n", (unsigned long)GetCurrentProcessId());
        printf("  Parent Process PID : %s\n", argv[2]);
        shared_variable += 50;
        printf("  Child modified shared_variable to: %d\n", shared_variable);
        Sleep(500);
        printf("  Child process exiting.\n");
        return 0;
    }
#else
    (void)argc;
    (void)argv;
#endif

    printf("=================================================================\n");
    printf("        PROCESS CREATION AND FORK SIMULATION IN C\n");
    printf("=================================================================\n");

#ifdef _WIN32
    run_windows_process_demo(argv[0]);
#else
    run_posix_fork_demo();
#endif

    printf("=================================================================\n");
    printf("Theory Summary:\n");
    printf("  1. fork() creates a new process by duplicating the calling process.\n");
    printf("  2. In child:  fork() returns 0.\n");
    printf("  3. In parent: fork() returns PID of the newly created child.\n");
    printf("  4. Both parent and child run concurrently with private memory spaces.\n");
    printf("=================================================================\n");

    return 0;
}
