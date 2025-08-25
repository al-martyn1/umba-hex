/* #pragma pack(push,1) */
struct FrameOrg0
{
    std::uint8_t    header[6];                // 0 - 5
    std::uint8_t    paylod[1];                // 6
    std::uint32_t   crc;                      // 7 - 10

}; // struct FrameOrg0
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct Frame
{
    FrameOrg0       frameOrg0;                // ORG 00000000

}; // struct Frame
/* #pragma pack(pop) */

