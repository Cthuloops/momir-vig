/**
 * @file printer.h
 * @brief Printer header.
 *
 * Shout out to [Geoffry Hunter](https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/)
 * for the guide on how to set up the serial interface.
 */
#ifndef MOMIR_PRINTER_H_
#define MOMIR_PRINTER_H_

#include <stdbool.h>


/**
 *  @brief Represents a printer.
 */
struct Printer {
    const char *name;
    int fd;
};


enum ControlFlow {
    PRINTER_CONTROL_NONE,
    PRINTER_CONTROL_HARDWARE,
    PRINTER_CONTROL_SOFTWARE
};


enum DataBits {
    PRINTER_DATABITS_5,
    PRINTER_DATABITS_6,
    PRINTER_DATABITS_7,
    PRINTER_DATABITS_8
};


enum StopBits {
    PRINTER_STOPBITS_1,
    PRINTER_STOPBITS_2
};


enum BaudRate {
    PRINTER_BAUD_0,
    PRINTER_BAUD_50,
    PRINTER_BAUD_75,
    PRINTER_BAUD_110,
    PRINTER_BAUD_134,
    PRINTER_BAUD_150,
    PRINTER_BAUD_200,
    PRINTER_BAUD_300,
    PRINTER_BAUD_600,
    PRINTER_BAUD_1200,
    PRINTER_BAUD_1800,
    PRINTER_BAUD_2400,
    PRINTER_BAUD_4800,
    PRINTER_BAUD_9600,
    PRINTER_BAUD_19200,
    PRINTER_BAUD_38400
};


enum Parity {
    PRINTER_PARITY_OFF,
    PRINTER_PARITY_ODD,
    PRINTER_PARITY_EVEN
};


/**
 *  @brief Transient struct for initializing a printer.
 *
 *  This struct is only supposed to be used for initializing a printer to the
 *  specified settings.
 */
struct PrinterSettings {
    const char *device;
    enum ControlFlow flow;
    enum DataBits data_bits;
    enum StopBits stop_bits;
    enum BaudRate baud_rate;
    enum Parity parity;
};


/**
 * Open a printer.
 *
 * @param[in] printer Printer to initialize.
 * @param[in] settings Settings to initialize the printer to.
 *
 * @return True if successful, False otherwise.
 */
bool printer_init(struct Printer *printer,
        const struct PrinterSettings *settings);


/**
 * Close a printer.
 *
 * @param[in] printer Printer to deinitialize.
 *
 * @return True if no errors occurred, False otherwise.
 */
bool printer_deinit(struct Printer *printer);


#endif  // MOMIR_PRINTER_H_
