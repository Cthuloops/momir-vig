# Momir Vig

## Overview
A C-based Linux application designed to fetch Magic: The Gathering card data via the Scryfall API and output it directly to a BTP-R180II thermal receipt printer. Built specifically to facilitate tabletop Momir Basic gameplay, this project bridges modern REST APIs with low-level RS232 hardware interfacing.

## Features
* **Automated API Synchronization:** Safely downloads and manages Scryfall bulk JSON data utilizing `libcurl`.
* **Robust Data Parsing:** Employs `cJSON` for memory-safe extraction of card attributes and resource URIs.
* **Hardware Interfacing:** Direct serial communication utilizing POSIX `termios`, sending customized ESC/POS byte commands formatted for 180 DPI rendering on 57.5mm thermal paper.
* **Defensive C Design:** Strict pointer validation macros, a custom multi-level logging system, and a strictly modular architecture.
* **Test-Driven:** Includes automated unit testing using the Unity framework to ensure parsing stability.

## Architecture
The project is decoupled into discrete modules:
* `http_client`: Wraps `libcurl` for reliable, memory-managed HTTP requests.
* `bulk_data`: Handles local storage, cache expiration (7-day lifecycle), and JSON parsing.
* `printer`: Manages RS232 baud rates, bit parity, and hardware flow control for thermal printers.
* `logging`: A custom logger tracking execution state and module-level errors.

## Development Environment
Optimized for Linux system development (e.g., Raspberry Pi) managing dedicated serial connections. The project relies on standard GCC toolchains, AddressSanitizer for memory profiling, and custom Makefiles. 

## Building the Project
Ensure `libcurl` and standard build essentials are installed on your Linux environment.

```bash
# Compile the main application
make

# Execute the binary
./momir
```

To run the automated unit test suite:
```bash
cd tests
make test
```

## Future Roadmap
* Implement image downloading and bitmap dithering to translate full-color Scryfall art into monochrome thermal prints.
* Create SQLite database for card information.

## License
Refer to the `LICENSE` file for more information.
