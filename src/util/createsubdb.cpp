#include <climits>
#include <fstream>

#include "Parameters.h"
#include "DBReader.h"
#include "DBWriter.h"
#include "Debug.h"

int createsubdb(int argc, const char **argv, const Command& command) {
    Parameters& par = Parameters::getInstance();
    par.parseParameters(argc, argv, command, 3);

    DBReader<unsigned int> reader(par.db2.c_str(), par.db2Index.c_str());
    reader.open(DBReader<unsigned int>::NOSORT);

    DBWriter writer(par.db3.c_str(), par.db3Index.c_str());
    writer.open();

    Debug(Debug::INFO) << "Start writing to file " << par.db3 << "\n";
    std::ifstream  orderFile(par.db1);
    std::string line;
    while(std::getline(orderFile, line)) {
        const unsigned int key = (unsigned int) strtoul(line.c_str(), NULL, 10);
        size_t id = reader.getId(key);
        if(id >= UINT_MAX) {
            Debug(Debug::WARNING) << "Key " << line << " not found in database\n";
            continue;
        }

        const char* data = reader.getData(id);
        // discard null byte
        size_t length = reader.getSeqLens(id) - 1;
        writer.writeData(data, length, key);
    }
    orderFile.close();
    writer.close();
    reader.close();

    return EXIT_SUCCESS;
}
