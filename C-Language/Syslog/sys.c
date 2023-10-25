#include <stdio.h>
#include <syslog.h>
#include <stdarg.h>
#include <string.h>

void syslog_custom(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    // 逐行輸出到 syslog
    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        syslog(LOG_INFO, "%s", line);
        line = strtok(NULL, "\n");
    }

    va_end(args);
}

int main() {
    // 假設有一個訊息包含換行符號
    const char *message = "This is a multi-line\nmessage.";

    openlog("[GGG] OMCID", LOG_PID, LOG_USER);
    // 使用自定義的 syslog 函數
    syslog_custom(message);

    closelog();
    return 0;
}
