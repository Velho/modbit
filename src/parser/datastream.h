#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <vector>
#include <memory>
#include <functional>

// Windows specific headers.

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <windows.h>

namespace modbit {

/**
 * Streams the output from Web.
 * Fills the buffer with data at each timeout
 * interval. Retrieved data is kept in memory so
 * no Web data is saved in filesystem.
 */
class DataStream {
public:
    explicit DataStream(std::string&);
    ~DataStream();

    // Disallow copying the DataStream.
    DataStream(DataStream&) = delete;
    DataStream &operator=(DataStream&) = delete;

    // Aliases for DataStream to work with.
    // BufferEvent is fired when BufferData is read.
    using BufferData = std::vector<char*>;
    using CBBufferEvent = std::function<void(BufferData&&)>;

    /**
     * Callback which is called when all data
     * is read.
     */
    CBBufferEvent OnBufferData;

    /**
     * Initializes the socket and
     * open up the connection.
     * \return True if the Socket library was init correctly.
     */
    bool Open();

    void RequestData();

    /**
     * Returns the vector of Errors if everythings
     * goes downhill. Last error won't do much but
     * whole log can help. It can be printed on the
     * GUI aswell.
     * \return Reference to the error log output from Sockets.
     */
    const std::vector<std::string> &GetErrorLog();

    /**
     * Reads the data from Socket to BufferData.
     * Requires socket to be initialized.
     * \return Size of read data.
     */
    size_t Read();

    /** 
     * Writes the provided data to the socket as BufferData.
     * Requires socket to be initialized.
     *
     * \param Pointer to the BufferData containing the data to be written.
     * \return Returns true if the writing was successfull.
     */
    bool Write(BufferData*);

private:
    unsigned int m_mstimeout = 5000;
    std::vector<std::string> m_errorlog;
    std::string m_host;

    bool m_initialized;
    SOCKET m_clientsock;
    WSADATA m_socketdata;
    addrinfo m_hostspecs, *m_results, *m_addr;
    hostent *m_remotehost;

   /**
    * Creates ipv4 socket.
    *
    *\return True if the socket creation is successfull,
    * Invalid Socket will lead to false.
    */
    bool CreateSocket();

    /**
     * Initializes the server information
     * from where to retrive the requried
     * data. Doesn't return anything because
     * only initializing some variables in struct.
     *
     *\return True if Server/Connection information was
     * successfully initialized.
     */
    bool InitServerInfo();
};
} // DATASTREAM_H
#endif
