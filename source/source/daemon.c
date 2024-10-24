#include "../include/daemon.h"

int become_daemon()
{   
    int maxfd, fd;
    switch(fork()){
    case -1: 
        return -1;
    case 0:
        break;
    default: _exit(EXIT_SUCCESS);
    }
    if(setsid() == -1)
        return -1;
    switch(fork()){
        case -1: 
            return -1;
        case 0:
            break;
        default: _exit(EXIT_SUCCESS);
    }
    if(!(BD_NO_UMASK0))
        umask(0);
    if(!(BD_NO_CHDIR))
        chdir("/");
    if(!(BD_NO_CLOSE_FILES)){ 
        maxfd = sysconf(_SC_OPEN_MAX);
        if(maxfd == -1)
            maxfd = BD_MAX_CLOSE;
        for(fd = 0; fd < maxfd; fd++)
            close(fd);
    }
    if(!(BD_NO_REOPEN_STD_FDS)){
        close(STDIN_FILENO);
        fd = open("/dev/null", 02);
        if(fd != STDIN_FILENO)
        return -1;
        if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
        return -2;
        if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
        return -3;
    }
    return 0;
}
int daemon_initialize()
{
    int ret;
    const char *LOGNAME = "mqttbd";
    ret = become_daemon();
    if (ret != 0){
        syslog(LOG_USER | LOG_ERR, "Daemon error while starting");
        closelog();
        return EXIT_FAILURE;
    }
    openlog(LOGNAME, LOG_PID, LOG_USER);
    syslog(LOG_USER | LOG_INFO, "Daemon starting");
    return 0;
}