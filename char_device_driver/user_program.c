#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE "/dev/my_char_device"

int main() {
    int fd;
    char write_buf[100];
    char read_buf[100];

    // Open the character device
    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }
    printf("Device opened successfully: %s\n", DEVICE);

    // Get user input to write to the device
    printf("Enter message to send: ");
    fgets(write_buf, sizeof(write_buf), stdin);
    write_buf[strcspn(write_buf, "\n")] = 0;

    // Write to the device
    if (write(fd, write_buf, strlen(write_buf)) < 0) {
        perror("Failed to write to device");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Message written to device: %s\n", write_buf);

    // Read from the device
    if (read(fd, read_buf, sizeof(read_buf)) < 0) {
        perror("Failed to read from device");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Message read from device: %s\n", read_buf);

    close(fd);
    printf("Device closed\n");

    return EXIT_SUCCESS;
}
