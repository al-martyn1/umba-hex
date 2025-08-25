/* #pragma pack(push,1) */
struct HarwareInfo
{
    std::char       deviceName[14];           // 0 - 13
    std::uint64_t   uint64;                   // 14 - 21
    std::uint8_t    hwHi;                     // 22
    std::uint8_t    hwLo;                     // 23
    std::uint16_t   manY;                     // 24 - 25
    std::uint8_t    manM;                     // 26
    std::uint8_t    manD;                     // 27
    std::uint16_t   batch;                    // 28 - 29
    std::uint8_t    serialNumber[8];          // 30 - 37
    std::uint16_t   signatureLe;              // 38 - 39
    std::uint16_t   signatureBe;              // 40 - 41

}; // struct HarwareInfo
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct SoftwareInfo
{
    std::uint16_t   firmwareVersionMajor;     // 0 - 1
    std::uint16_t   firmwareVersionMinor;     // 2 - 3
    std::uint16_t   firmwareVersionBuild;     // 4 - 5
    std::uint16_t   buildYear;                // 6 - 7
    std::uint8_t    buildMonth;               // 8
    std::uint8_t    buildDay;                 // 9
    std::uint8_t    buildHour;                // 10
    std::uint8_t    buildMinute;              // 11
    char            versionInfoString[16];    // 12 - 27
    char            builtDateTimeString[16];  // 28 - 43
    char            signleChar1;              // 44
    std::uint64_t   leTest;                   // 45 - 52
    char            signleChar2;              // 53
    std::uint64_t   beTest;                   // 54 - 61
    char            signleChar3;              // 62
    std::uint64_t   meTest;                   // 63 - 70
    char            signleChar4;              // 71
    std::uint64_t   testArray4OfUint64[4];    // 72 - 103
    char            signleChar5;              // 104
    std::uint32_t   testArray8OfUint32[8];    // 105 - 136
    char            fill[7];                  // 137 - 143

}; // struct SoftwareInfo
/* #pragma pack(pop) */


/* #pragma pack(push,1) */
struct TestExample
{
    Harware Info    harwareInfo;              // ORG 1000:FF40
    std::uint8_t    fill0[6];                 // 42 - 47
    Software Info   softwareInfo;             // ORG 1000:FF70

}; // struct TestExample
/* #pragma pack(pop) */

