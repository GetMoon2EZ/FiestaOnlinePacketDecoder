#include "fopd/fopd_utils.h"

#include <cstdint>
#include <cstdlib>
#include <vector>

#include <winsock2.h>
#include <winbase.h>
#include <errhandlingapi.h>

#include "fopd/fopd_types.h"
#include "fopd/fopd_consts.h"


using namespace std;

#define UNKNOWNENDIAN   0xFFFF

uint16_t endianess = UNKNOWNENDIAN;

uint32_t little_endian_byte_array_to_uint32(uint8_t *byte_array)
{
    uint32_t ret = 0;
    for (int i = 0; i < 4; i++) {
        ret = ret << 8;
        ret += byte_array[3 - i];
    }

    return ret;
}

uint16_t little_endian_byte_array_to_uint16(uint8_t *byte_array)
{
    uint16_t ret = 0;
    for (int i = 0; i < 2; i++) {
        ret = ret << 8;
        ret += byte_array[1 - i];
    }

    return ret;
}

fopd_packet_type_t packetTypeFromHeader(uint8_t header[FOPD_PACKET_HEADER_LEN])
{
    if (memcmp(header, FOPD_SPELL_DAMAGE_PACKET_HEADER, FOPD_PACKET_HEADER_LEN) == 0 || memcmp(header, FOPD_AA_DAMAGE_PACKET_HEADER, FOPD_PACKET_HEADER_LEN) == 0) {
        return FOPD_DAMAGE_PACKET;
    }
    else if (memcmp(header, FOPD_ENTITY_CLICK_PACKET_HEADER, FOPD_PACKET_HEADER_LEN) == 0) {
        return FOPD_ENTITY_CLICK_PACKET;
    }
    else {
        return FOPD_UNKNOWN_PACKET;
    }
}

fopd_packet_type_t packetTypeFromData(uint8_t *data)
{
    uint8_t header[FOPD_PACKET_HEADER_LEN] = { 0 };
    memcpy(header, &data[FOPD_PACKET_HEADER_OFFSET], FOPD_PACKET_HEADER_LEN);
    return packetTypeFromHeader(header);
}

vector<pair<fopd_packet_type_t, vector<uint8_t>>> getPacketsFromRawTCP(uint8_t *tcp_data, uint32_t tcp_data_len)
{
    if (tcp_data == NULL)
        return vector<pair<fopd_packet_type_t, vector<uint8_t>>> ();

    std::vector<std::pair<fopd_packet_type_t, std::vector<uint8_t>>> ret;
    uint32_t pos = 0;

    while (pos < tcp_data_len) {
        uint16_t payload_len = ((uint16_t) tcp_data[pos + FOPD_PACKET_PAYLOAD_LEN_OFFSET]) + 1;

        if (payload_len == 0 || pos + payload_len > tcp_data_len) {
            break;
        }

        std::vector<uint8_t> payload(&tcp_data[pos], &tcp_data[pos + payload_len]);

        // Get packet type
        fopd_packet_type_t type = packetTypeFromData(&tcp_data[pos]);

        ret.push_back(pair<fopd_packet_type_t, vector<uint8_t>>(type, payload));

        // Go to the next packet
        pos += payload_len;
    }

    return ret;
}

char *vec_u8_to_hex_str(std::vector<uint8_t> v, size_t *s)
{
    char *ret = NULL;
    char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    *s = v.size() * 2 + 1;
    ret = (char *) calloc(*s, sizeof(uint8_t));
    for (size_t i = 0; i < v.size(); i++) {
        ret[i * 2] = hex[v[i] >> 4];
        ret[i * 2 +  1] = hex[v[i] & 0x0F];
    }
    ret[*s-1] = '\0';

    return ret;
}

int
timeval_subtract(struct timeval x, struct timeval y, struct timeval *result)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x.tv_usec < y.tv_usec) {
        int nsec = (y.tv_usec - x.tv_usec) / 1000000 + 1;
        y.tv_usec -= 1000000 * nsec;
        y.tv_sec += nsec;
    }
    if (x.tv_usec - y.tv_usec > 1000000) {
        int nsec = (x.tv_usec - y.tv_usec) / 1000000;
        y.tv_usec += 1000000 * nsec;
        y.tv_sec -= nsec;
    }

    /* Compute the time remaining to wait. tv_usec is certainly positive. */
    result->tv_sec = x.tv_sec - y.tv_sec;
    result->tv_usec = x.tv_usec - y.tv_usec;

    /* Return 1 if result is negative. */
    return x.tv_sec < y.tv_sec;
}

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}

void
print_system_error(const char *func_name)
{
    DWORD err_code = GetLastError();
    LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL
    );

    fprintf(stderr, "[ERROR] %s returned error %d: %s\n", func_name, err_code, (PTSTR) lpMsgBuf);
    LocalFree(lpMsgBuf);
}

uint16_t
letoss(uint16_t x)
{
    if (endianess == UNKNOWNENDIAN) {
        int n = 1;
        endianess = *(char *)&n == 1 ? LITTLEENDIAN : BIGENDIAN;
    }

    return endianess == BIGENDIAN ? ntohs(x) : x;
}

uint32_t
letosl(uint32_t x)
{
    if (endianess == UNKNOWNENDIAN) {
        int n = 1;
        endianess = *(char *)&n == 1 ? LITTLEENDIAN : BIGENDIAN;
    }

    return endianess == BIGENDIAN ? ntohl(x) : x;
}
