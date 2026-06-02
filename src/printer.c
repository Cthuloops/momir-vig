#define _DEFAULT_SOURCE  // Included for CRTSCTS flag

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "printer.h"


static const tcflag_t data_bits[] = { CS5, CS6, CS7, CS8 };
static const speed_t baud_rate[] = {
    B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400,
    B4800, B9600, B19200, B38400
};


bool printer_init(struct Printer *printer,
        const struct PrinterSettings *settings) {
    bool status_out = false;

    if (printer == NULL) {
        fprintf(stderr, "Passed NULL printer struct pointer to printer_init\n");
        goto out;
    }

    if (printer->name == NULL) {
        fprintf(stderr, "No name provided for Printer\n");
        goto out;
    }

    if (settings == NULL) {
        fprintf(stderr, "Must supply printer settings for printer %s init.\n",
                printer->name);
        goto out;
    }

    if (settings->device == NULL) {
        fprintf(stderr, "Device path for printer %s is NULL.\n", printer->name);
        goto out;
    }

    int port = open(settings->device, O_RDWR);

    if (port < 0) {
        fprintf(stderr, "Error %i from open: %s\n", errno, strerror(errno));
        goto cleanup;
    }

    struct termios tty;
    if (tcgetattr(port, &tty) != 0) {
        fprintf(stderr, "Error %i from tcgetattr: %s\n", errno,
                strerror(errno));
        goto cleanup;
    }

    switch (settings->flow) {
        case PRINTER_CONTROL_NONE:
            tty.c_cflag &= ~CRTSCTS;
            break;

        case PRINTER_CONTROL_HARDWARE:
            tty.c_cflag |= CRTSCTS;
            break;

        case PRINTER_CONTROL_SOFTWARE:
            tty.c_cflag &= ~CRTSCTS;
            tty.c_cflag &= (IXON | IXOFF | IXANY);
            break;

        default:
            fprintf(stderr,
                    "Incorrect value %d supplied for printer %s control flow "
                    "setting\n", settings->flow, printer->name);
            goto cleanup;
    }

    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= data_bits[settings->data_bits];

    switch (settings->stop_bits) {
        case PRINTER_STOPBITS_1:
            tty.c_cflag &= ~CSTOPB;
            break;

        case PRINTER_STOPBITS_2:
            tty.c_cflag |= CSTOPB;
            break;

        default:
            fprintf(stderr,
                    "Incorrect value %d supplied for printer %s stop bits.\n",
                    settings->stop_bits, printer->name);
            goto cleanup;
    }

    cfsetispeed(&tty, baud_rate[settings->baud_rate]);
    cfsetospeed(&tty, baud_rate[settings->baud_rate]);

    switch (settings->parity) {
        case PRINTER_PARITY_OFF:
            tty.c_cflag &= ~PARENB;
            break;

        case PRINTER_PARITY_ODD:
            tty.c_cflag |= PARODD;
            break;

        case PRINTER_PARITY_EVEN:
            break;

        default:
            fprintf(stderr,
                    "Invalid value %d supplied for printer %s parity.\n",
                    settings->parity, printer->name);
            goto cleanup;
    }

    // These settings may be specific to the current printer that I have but,
    // I'll worry about that when the time comes.
    tty.c_cflag |= CREAD | CLOCAL;

    // Essentially raw mode.
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~(ECHO & ECHOE & ECHONL);
    tty.c_lflag &= ~ISIG;

    // Disable special handling of received bytes.
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    tty.c_oflag &= ~OPOST;  // Prevent special interpetation of output bytes.
    tty.c_oflag &= ~ONLCR;  // Prevent conversion of \n to \r\lf.

    if (tcsetattr(port, TCSANOW, &tty) != 0) {
        fprintf(stderr, "Error %i from tcsetattr: %s\n", errno,
                strerror(errno));
        goto cleanup;
    }

    printer->fd = port;

    status_out = true;
    goto out;

cleanup:
    if (port != -1) {
        close(port);
    }
    printer->fd = -1;

out:
    return status_out;
}


bool printer_deinit(struct Printer *printer) {
    bool status_out = false;

    if (close(printer->fd) == -1) {
        fprintf(stderr, "Error %i from close: %s\n", errno, strerror(errno));
        goto out;
    }

    printer->name = NULL;
    printer->fd = -1;
    status_out = true;

out:
    return status_out;
}
