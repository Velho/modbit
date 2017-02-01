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

    /*
    // Disallow copying the DataStream.
    DataStream(DataStream&) = delete;
    DataStream &operator=(DataStream&) = delete;
    */

    // Aliases for DataStream to work with.
    // BufferEvent is fired when BufferData is read.
    typedef std::vector<char*> BufferData;
    typedef std::function<void(std::string&&)> CBBufferEvent;
    
    // Required information from response.
    enum class StatusCode {
        OK = 200, NOT_MODIFIED = 304
    };

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

    /**
     * Sets the running flag to false.
     */
    void Close();

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
     StatusCode Read();

    /** 
     * Writes the provided data to the socket as BufferData.
     * Requires socket to be initialized.
     *
     * \return Returns true if the writing was successfull.
     */
    bool Write();

private:
    unsigned int m_mstimeout = 25000;
    std::vector<std::string> m_errorlog;
    std::string m_host;

    bool m_running;
    bool m_initialized;
    bool m_initsocket;
    SOCKET m_clientsock;
    WSADATA m_socketdata;
    addrinfo m_hostspecs, *m_results, *m_addr;
    hostent *m_remotehost;

    BufferData response;

    std::string GetRequestHeader();

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


    StatusCode GetResponseStatus(std::string&);
};
} // DATASTREAM_H
#endif
