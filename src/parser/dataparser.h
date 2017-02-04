#ifndef DATAPARSER_H

#include "datastream.h"

#include <memory>
#include <thread>

namespace modbit {

class Modbus;

/**
 * Parses the incoming Data from the stream.
 */
class DataParser {
public:
    DataParser();
    ~DataParser();

    using DataStorage = std::vector<std::string*>;
	using ShutdownEvent = std::function<void(bool)>;

	ShutdownEvent OnShutdownEvent;

    /**
     * Connects the DataStreams callback
     * to the parser.
     */
    void Connect();
	void Disconnect();

    int GetIdx() { return m_parseridx; }
    Modbus *GetData();

private:
    DataStream *m_stream;
    std::thread m_streamthread;
    DataStorage m_storage;
    bool m_threadrunning;
    
    std::unique_ptr<Modbus> m_data;

    int m_parseridx;
    int m_storagesz;

    /**
     * Event Callback for OnBufferData.
     * When Data is received HandleData manages
     * and correctly stores it for future.
     */
    void HandleData(std::string&&);
    void ParseData();

	void CloseStream();
};

}

#endif // !DATAPARSER_H
