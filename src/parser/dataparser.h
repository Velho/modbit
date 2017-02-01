#ifndef DATAPARSER_H

#include "datastream.h"

#include <memory>
#include <thread>

namespace modbit {

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

private:
    DataStream *m_stream;
    std::thread m_streamthread;
    DataStorage m_storage;
    std::string m_data;
    bool m_threadrunning;

    /**
     * Event Callback for OnBufferData.
     * When Data is received HandleData manages
     * and correctly stores it for future.
     */
    void HandleData(std::string&&);

	void CloseStream();
};

}

#endif // !DATAPARSER_H
