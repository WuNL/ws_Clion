#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <execinfo.h>
#include <sys/time.h>

/* 定义一个数据结构用来保存信号 */
typedef struct sigInfo
{
    int signum;
    char signame[20];
} sigInfo;

/* 增加我们想要捕捉的异常信号，这里列举了6个 */
sigInfo sigCatch[] = {
        {1,  "SIGHUP"},
        {2,  "SIGINT"},
        {3,  "SIGQUIT"},
        {6,  "SIGABRT"},
        {8,  "SIGFPE"},
        {11, "SIGSEGV"}
};

/* 我们自定义的信号处理函数 */
void blackbox_handler(int sig)
{
    printf("Enter blackbox_handler: ");
    printf("SIG name is %s, SIG num is %d\n", strsignal(sig), sig);
    // 打印堆栈信息
    printf("Stack information:\n");
    int j, nptrs;

#define SIZE 100
    void *buffer[100];

    char **strings;
    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        perror("backtrace_symbol");
        exit(EXIT_FAILURE);
    }
    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    free(strings);
    _exit(EXIT_SUCCESS);
}


/* 有bug的程序，调用该程序，将随机产生一些异常信号 */

void bug_func()
{
    void *buffer = malloc(100);
    char s[1000];
    memset(s, 1, sizeof(s) * 5);
    memcpy(buffer, s, sizeof(s) * 5);

    printf("my bug\n");
    return;


    int rand;
    struct timeval tpstart;
    pid_t my_pid = getpid();

    // 产生随机数
    gettimeofday(&tpstart, NULL);
    srand(tpstart.tv_usec);
    while ((rand = random()) > (sizeof(sigCatch) / sizeof(sigInfo)));
    printf("rand=%d\n", rand);
    //随机产生异常信号
    switch (rand % (sizeof(sigCatch) / sizeof(sigInfo)))
    {
        case 0:
        {
            // SIGHUP
            kill(my_pid, SIGHUP);
            break;
        }
        case 1:
        {
            // SIGINT
            kill(my_pid, SIGINT);
            break;
        }
        case 2:
        {
            // SIGQUIT
            kill(my_pid, SIGQUIT);
            break;
        }
        case 3:
        {
            // SIGABRT
            abort();
            break;
        }
        case 4:
        {
            // SIGFPE
            int a = 6 / 0;
            break;
        }
        case 5:
        {
            // SIGSEGV
            kill(my_pid, SIGSEGV);
            break;
        }
        default:
            return;
    }
}

int main()
{
    int i, j;
    struct sigaction sa;

    // 初始化信号处理函数数据结构
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = blackbox_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    for (i = 0; i < sizeof(sigCatch) / sizeof(sigInfo); i++)
    {
        // 注册信号处理函数
        if (sigaction(sigCatch[i].signum, &sa, NULL) < 0)
        {
            return EXIT_FAILURE;
        }
    }
    bug_func();
    while (1);
    return EXIT_SUCCESS;

}