
#include "fopd/fopd_consts.h"

#include <string>

// Server address, change according to server
// Cypian = "35.189.254.31" 
// Turn into map for release
const std::string FIESTA_ONLINE_SERVER_ADDRESS = "35.189.254.31";

const uint8_t FOPD_SPELL_DAMAGE_PACKET_HEADER[FOPD_PACKET_HEADER_LEN] = {
                                                                            FOPD_SPELL_DAMAGE_PACKET_HEADER_B0,
                                                                            FOPD_SPELL_DAMAGE_PACKET_HEADER_B1
                                                                        };

const uint8_t FOPD_AA_DAMAGE_PACKET_HEADER[FOPD_PACKET_HEADER_LEN]    = {
                                                                            FOPD_AA_DAMAGE_PACKET_HEADER_B0,
                                                                            FOPD_AA_DAMAGE_PACKET_HEADER_B1
                                                                        };

const uint8_t FOPD_ENTITY_CLICK_PACKET_HEADER[FOPD_PACKET_HEADER_LEN] = {
                                                                            FOPD_ENTITY_CLICK_PACKET_HEADER_B0,
                                                                            FOPD_ENTITY_CLICK_PACKET_HEADER_B1
                                                                        };
