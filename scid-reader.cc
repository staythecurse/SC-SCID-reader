#include <iostream>
#include "sierra-scid-reader.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <DATAFILEPATH>" << std::endl;
        return 1;
    }

    std::vector<std::string> dataFiles;

    for (int i=1; i < argc; i++)
    {
        dataFiles.push_back(argv[i]);
    }

    std::vector<s_IntradayRecord> ir = read_scid(dataFiles);

    // This needs to display with microsecond accuracy
    // using the SCDateTime epoch of December 30, 1899.
    // but I cannot figure that out to save my life
    time_t dateTime = ir[46].DateTime.ToUNIXTime();
    struct tm * timeinfo = gmtime(&dateTime);
    char dateString [80];
    strftime(dateString,80, "%Y-%m-%d %H:%M:%S", gmtime(&dateTime));
    std::cout << "\n       v[46].Date: " << dateString << std::endl;

    std::cout << "\n       v[46].Open: " << ir[46].Open << std::endl;
    std::cout << "       v[46].High: " << ir[46].High << std::endl;
    std::cout << "        v[46].Low: " << ir[46].Low << std::endl;
    std::cout << "      v[46].Close: " << ir[46].Close << std::endl;

    // for tick data NumTrades should always be 1
    std::cout << "\n  v[46].NumTrades: " << ir[46].NumTrades << std::endl;
    std::cout << "v[46].TotalVolume: " << ir[46].TotalVolume << std::endl;
    std::cout << "  v[46].BidVolume: " << ir[46].BidVolume << std::endl;
    std::cout << "  v[46].AskVolume: " << ir[46].AskVolume << std::endl;

    return 0;
}
