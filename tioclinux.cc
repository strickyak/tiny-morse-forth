#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>



// int ioctl(int fd, unsigned long request, ...);


int main() {
  int console = open("/dev/tty", O_RDWR, 0777);
  if (console < 0) { perror("/dev/tty"); return errno; }
  while(1) {

    constexpr char SHIFT_STATE = 6;
    struct {char subcode;} arg {SHIFT_STATE};
    int err = ioctl(console, TIOCLINUX, &arg);
    if (err) { perror("ioctl"); return errno; }

    printf("%d ", arg.subcode);
    fflush(stdout);
    system("sleep 1");
  }
}
