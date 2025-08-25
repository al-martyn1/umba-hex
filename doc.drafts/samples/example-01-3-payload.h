/* #pragma pack(push,1) */
struct PayloadOrg0
{
    std::uint8_t    functionId;               // 0
    std::uint8_t    data[1];                  // 1

}; // struct PayloadOrg0
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct Payload
{
    PayloadOrg0     payloadOrg0;              // ORG 00000000

}; // struct Payload
/* #pragma pack(pop) */

