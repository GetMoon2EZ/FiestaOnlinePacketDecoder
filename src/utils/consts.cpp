
#include "fopd/fopd_consts.h"

#include <string>

// Server address, change according to server
// Cypian = "35.189.254.31" 
// Turn into map for release
const std::string FIESTA_ONLINE_SERVER_ADDRESS = "35.189.254.31";

const uint8_t FOPD_ENTITY_CLICK_PACKET_HEADER[FOPD_PACKET_HEADER_LEN] = { 0x02, 0x24 };
const uint8_t FOPD_DAMAGE_PACKET_HEADER[FOPD_PACKET_HEADER_LEN] = { 0x00, 0x00 };
