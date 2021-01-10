// Basic code required to read and output
// a SierraChart SCID data file

// I realize that the excessive comments
// are not considered good coding practice.
// The extensive comments are provided in hopes
// that it will make understanding the code
// easy for C++ noobs as well as to refresh
// my own memory if I step away from this
// project for an extensive period of time
// as I myself am but a novice with C++

// This is also the bare minimum code
// you are responsible for adding whatever
// error handling is necessary for your
// usage.

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
// This preprocessor directive is ignored
// if you are compiling for a platform
// other than linux
#ifdef __linux__
    #define stricmp strcasecmp
    #define strnicmp strncasecmp
#endif

// These includes are located in your SierraChart installation directory
// Copy them to a known location and reference them accordingly or
// just add the SierraChart/ACS_Source directory to your include path
#include "../libs/ACS_Source/scdatetime.h"
#include "../libs/ACS_Source/IntradayRecord.h"


// special records defined in by s_IntradayRecord
const float FIRST_SUB_TRADE_OF_UNBUNDLED_TRADE = -1.99900095e+37F;
const float LAST_SUB_TRADE_OF_UNBUNDLED_TRADE = -1.99900197e+37F;

// SCID file structure
// 1 (one) s_IntradayFileHeader at the beginning of the file
// all subsequent records are s_IntradayRecord
// headerSize = 56
// intradayRecordSize = 40
// number of Intraday records in scid file = (fileSize - headerSize) / intradayRecordSize


// read_scid function takes string vector of SCID data file paths as input
// and returns a vector of s_Intradarecord that can be accessed like an array
std::vector<s_IntradayRecord> read_scid(std::vector<std::string> dataFiles)
{
    // create a local tsData vector
    std::vector<s_IntradayRecord> tsData;

    // loop through the list of data files
    // and read their data into the tsData vector
    for (int i=0; i < dataFiles.size(); i++)
    {
        // create local structs based on the IntradayRecord.h definition
        s_IntradayFileHeader irh;
        s_IntradayRecord     ir;

        // create a read only file handle
        std::ifstream scidFile;
        // Open file in binary mode and immediately seek to the end
        // Segmentation fault if the file does not exist
        scidFile.open(dataFiles[i], std::ios::binary | std::ios::ate);

        // Measure the file size and calculate number of s_IntradayRecord
        // You could (should) test file size before processing it to make
        // sure you have enough memory to available to continue but that
        // functionality is beyond the scope of this function for now
        long long fsize = scidFile.tellg();
        int recordCount = ((fsize - 56) / 40) + 1;
        std::cout << recordCount << " records to read" << std::endl;

        scidFile.clear(); // clear EOF and FAIL flags that were set by seeking to EOF
        scidFile.seekg(0); // go back to the beginning of the file

        if (scidFile.is_open())
        {
            // Function fails here and throws "No such file or directory"
            // in GDB (Gnu Debugger) if an out of memory condition occurs

            // Read file header and do nothing with it
            // Technically we could just seek past the file
            // header but this makes it available should we
            // find a use for it later
            scidFile.read((char*)&irh, sizeof(irh));

            while (scidFile.good())
            {
                // read each record
                scidFile.read((char*)&ir, sizeof(ir));
                // and add it to tsData vector
                tsData.push_back(ir);
            }
            // close the file handle
            scidFile.close();
        }
    }

    return tsData;
}
