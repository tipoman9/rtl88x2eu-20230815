#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/wifi_tx_buffer_free_frames", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    /* Map the 4KB page. Our int is at the very beginning (offset 0). */
    volatile int *shared_val = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);
    
    if (shared_val == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("Reading from /dev/wifi_tx_buffer_free_frames at 400Hz...\n");

    while (1) {
        /* No syscall! This reads the memory directly. */
        printf("\rFree Count: %d    ", *shared_val);
        fflush(stdout);
        
        usleep(2500); // 400Hz
    }

    munmap((void*)shared_val, 4096);
    close(fd);
    return 0;
}