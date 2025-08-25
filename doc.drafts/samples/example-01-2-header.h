/* #pragma pack(push,1) */
struct HeaderOrg0
{
    std::uint8_t    preamble[2];              // 0 - 1
    std::uint16_t   remainingBytesCount;      // 2 - 3
    std::uint16_t   crc;                      // 4 - 5

}; // struct HeaderOrg0
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct Header
{
    HeaderOrg0      headerOrg0;               // ORG 00000000

}; // struct Header
/* #pragma pack(pop) */

