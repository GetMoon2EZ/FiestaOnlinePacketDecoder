#include "fopd/fopd_utils.h"

#include <vector>
#include <utility>
#include <cstdint>

#include <gtest/gtest.h>

#include "fopd/fopd_types.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

// TEST(PacketIdentification, DamagePacket) {
//   uint8_t pkt[] = { 0x14, 0x52, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//   std::vector<std::pair<fopd_packet_type_t, std::vector<uint8_t>>> r = getPacketsFromRawTCP(pkt, 0);

//   ASSERT_EQ(r.size(), 1);
//   ASSERT_EQ(r[0].first, FOPD_DAMAGE_PACKET);
// }

// TEST(Bytes, LittleEndianArrToInt32)
// {
//   uint8_t arr[] = { 0x00, 0x00, 0x00, 0x00 };
//   uint32_t r = little_endian_byte_array_to_uint32(arr);
//   EXPECT_EQ(r, (uint32_t) 0);
// }

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}