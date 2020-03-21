#ifndef SENSOR_DATA_SPOOF
#define SENSOR_DATA_SPOOF

#include <string>
#include <fstream>
#include <vector>
#include <limits> //for numeric_limits

#include "HAL/time.h"

// class for linking data streams to different sensors
// Inputted data to DataSpoof is in form of csv with first column
// being time in ms, and the other columns being each entry of data
// NOTE: The time in ms of the data must be aligned with the used time
template <std::size_t data_length>
class DataSpoof {
public:
    DataSpoof(std::string const &dataFile) {
        dataStream = std::ifstream(dataFile);

        if(!dataStream){
            std::cerr << "File could not be opened";
            std::exit(1);
        }
        // dlength = data_length;
        // data[dlength];
        // std::string dummy;
        // throw away first line of csv (headers)
        dataStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                char c;
    }

    // returns next data point in stream
    float* getData() {
        constexpr std::streamsize MAX_BUFF = 255;
        Hal::initialSystem();
        auto t_now = Hal::now_ms().time_since_epoch().count();
        while (t1 < t_now) {
            // std::vector<std::string> dataFields = split_string(dataStr);

            //get line
            char dat_line[MAX_BUFF];
            // std::getline(dataStream, dat_line);
            dataStream.getline(dat_line, MAX_BUFF);
            //neat trick - converting all of the commas to '\0' means we now get a set of c-style strings, all lined up
            char* data_strs[data_length + 1];
            data_strs[0] = dat_line;
            for(int i = 0, j = 0; dat_line[i] != '\0'; i++){
                if(dat_line[i] == ','){
                    dat_line[i] = '\0';
                    data_strs[++j] = dat_line + i + 1; //pointer to element after comma
                }
            }

            t0 = t1;
            t1 = std::stol(data_strs[0]);
            assert(t1 - t0 < 200); //Spacing between data points can't be too high otherwise sim becomes inaccurate

            for(int i = 0; i < data_length; i++) {
                dat_read[i] = std::stof(data_strs[i + 1]);
            }
        }

        return dat_read; //Not linearly interpolating b/c that is averaging, which reduces the noise level
    }

    int getDataLength() {return data_length;}

    ~DataSpoof() {
        dataStream.close();
    }
private:
    std::ifstream dataStream;
    long int t0, t1; //two variables exist only for the purpose of assert

    float dat_read[data_length];
};
#endif