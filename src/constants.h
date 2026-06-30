#ifndef MOMIR_CONSTANTS_H_
#define MOMIR_CONSTANTS_H_

#define MOMIR_DATA_DIR "./data/"
#define MOMIR_BULK_DATA_PATH MOMIR_DATA_DIR "bulk_data.json"

// HTTP_CLIENT
#define SCRYFALL_BULK_DATA_URL "https://api.scryfall.com/bulk-data"
#define SCRYFALL_USER_AGENT "User-Agent: MomirVigApp/0.1"
#define SCRYFALL_ACCEPT "Accept: */*"
#define SCRYFALL_BULK_DATA_UPDATE_TIME 604800 // Amount of seconds in 7 days.

// MODULES
#define MOMIR_MAIN "MOMIR_MAIN"
#define MOMIR_HTTP_CLIENT "MOMIR_HTTP_CLIENT"
#define MOMIR_PRINTER "MOMIR_PRINTER"
#define MOMIR_UTILS "MOMIR_UTILS"

#endif // MOMIR_CONSTANTS_H_
