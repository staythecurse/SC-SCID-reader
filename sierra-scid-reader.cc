// Basic code required to read and output
// a SierraChart SCID data file

// feedback is welcome - I love to learn

// Thank you to the SierraChart team for
// your excellent documentation of your
// APIs and data structures.

// SierraChart documentation of the SCID intraday file format
// https://www.sierrachart.com/index.php?page=doc/IntradayDataFileFormat.html


#include <climits> // used by SierraChart
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>


// These are necessary aliases on linux
// stricmp and strnicmp are Windows extensions
// (string ignore case compare)
#ifdef __linux__
    #define stricmp strcasecmp
    #define strnicmp strncasecmp
#endif

#include "../libs/ACS_Source/scdatetime.h"
#include "../libs/ACS_Source/IntradayRecord.h"


// special records defined in by s_IntradayRecord
const float FIRST_SUB_TRADE_OF_UNBUNDLED_TRADE = -1.99900095e+37F;
const float LAST_SUB_TRADE_OF_UNBUNDLED_TRADE = -1.99900197e+37F;

std::vector<s_IntradayRecord> read_scid(std::vector<std::string> dataFiles)
{
    std::vector<s_IntradayRecord> tsData;
    for (int i=0; i < dataFiles.size(); i++)
    {
        s_IntradayFileHeader irh;
        s_IntradayRecord     ir;

        std::ifstream scidFile;
        scidFile.open(dataFiles[i], std::ios::binary | std::ios::ate);

        long long fsize = scidFile.tellg();
        int recordCount = ((fsize - 56) / 40) + 1;
        std::cout << recordCount << " records to read" << std::endl;

        scidFile.clear(); // clear EOF and FAIL flags that were set by tellg()
        scidFile.seekg(0); // go back to the beginning of the file

        if (scidFile.is_open())
        {
            scidFile.read((char*)&irh, sizeof(irh));

            while (scidFile.good())
            {
                scidFile.read((char*)&ir, sizeof(ir));
                tsData.push_back(ir);
            }
            scidFile.close();
        }
    }

    return tsData;
}
