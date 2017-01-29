#include "datastream.h"

#include <string>

namespace modbit {

// Helper macros to shorten some calls.
// Reseting the socket just sets the new one as null which calls the .dtor
#define LOG_ERROR(FUNCTION, USERMSG) \
    m_errorlog.emplace_back(FUNCTION + " :: " + USERMSG + " -> " + std::to_string(WSAGetLastError()));

/* TODO : Perhaps use the Global Data to init the DataStream object. */
/* Global Data */
constexpr size_t g_PACKSZ { 512 };


// Underlying class provides high-level implementation
// of stream(without I/O operators for their high abstraction
// cost and Windows specific are stored here.


/**
 * .ctor
 * Initializes the pointer to structure and
 * provides the timeout for reading the output.
 */
DataStream::DataStream(std::string &host) :
    m_host { host }
{
    m_initialized = (WSAStartup(MAKEWORD(2, 2), &m_socketdata) == 0);
}

DataStream::~DataStream()
{
    if(m_initialized)
        WSACleanup();
}

bool DataStream::Open()
{
    // Create the socket and check if ctor was successful to init WinSock.
    if(!m_initialized && !CreateSocket()) {
        LOG_ERROR(std::string(__FUNCTION__), "Failed to initialize");
        WSACleanup();
        return false;
    }

    return true;
}

void DataStream::RequestData()
{
}

const std::vector<std::string> &DataStream::GetErrorLog()
{
    return m_errorlog;
}

size_t DataStream::Read()
{
    size_t recv_sz;
    BufferData data {}; // Empty our Buffer.

    // Loop the response from the server.
    do {
        recv_sz = recv(m_clientsock, *data.data(), g_PACKSZ, 0);
    } while(recv_sz > 0);

    OnBufferData(std::move(data));

	return data.size();
}

bool DataStream::Write(BufferData *data)
{
    /*
    int result = connect(
        m_clientsock, 
        reinterpret_cast<SOCKADDR*>(&m_socket->m_server),
        sizeof(m_socket->m_clientsock));

    if(result == SOCKET_ERROR) {
        closesocket(m_socket->m_clientsock);
        LOG_ERROR(std::string(__FUNCTION__), " Error Connecting socket");

        return false;
    }
*/

    if(send(m_clientsock, *data->data(), data->size(), 0) == SOCKET_ERROR) {
        LOG_ERROR(std::string(__FUNCTION__), "Unable to send data to server");
        WSACleanup();
        return false;
    }

    // Shutdown the socket but not yet closing as still using it,
    // though if failing to shutdown we have force cleanup.
    if(shutdown(m_clientsock, SD_SEND) == SOCKET_ERROR) {
        LOG_ERROR(std::string(__FUNCTION__), "");
        closesocket(m_clientsock);
        WSACleanup();
    }

	return true;
}

bool DataStream::CreateSocket()
{
    // Initializes the nonblocking flag and TCP Socket as Stream.
    u_long nonblock_flag = 1;

    // linked list to traverse to get the correct socket.
    for(m_addr = m_results; m_addr != nullptr;
            m_addr = m_addr->ai_next) {

        // Create socket.
        m_clientsock = socket(m_addr->ai_family, m_addr->ai_socktype, m_addr->ai_protocol);

        if(m_clientsock == INVALID_SOCKET) {
            LOG_ERROR(std::string(__FUNCTION__), "Socket failed to init");
            WSACleanup();
            return false;
        }

        int res = connect(m_clientsock, m_addr->ai_addr, m_addr->ai_addrlen);
        // Cleanup the old socket and continue looking for valid one.
        if(res == SOCKET_ERROR) {
            closesocket(m_clientsock);
            m_clientsock = INVALID_SOCKET;

            continue;
        }
        break;
    }

    freeaddrinfo(m_results);

    if(m_clientsock == INVALID_SOCKET) {
        LOG_ERROR(std::string(__FUNCTION__), "Unable to connect to server");
        WSACleanup();
        return false;
    }

    // Set socket as nonblocking.
    if(ioctlsocket(m_clientsock, FIONBIO, &nonblock_flag) != NO_ERROR) {
        LOG_ERROR(std::string(__FUNCTION__), "Not able to set Socket non-blocking");
        return false;
    }

    return true;
}

bool DataStream::InitServerInfo()
{
/*
    char *ip;
    m_socket->m_remotehost = gethostbyname(m_host.data());
    ip = inet_ntoa(*(reinterpret_cast<in_addr*>(*m_socket->m_remotehost->h_addr_list)));
*/

    ZeroMemory(&m_hostspecs, sizeof(m_hostspecs));
    
    m_hostspecs.ai_socktype = SOCK_STREAM;
    m_hostspecs.ai_family = AF_UNSPEC;
    m_hostspecs.ai_protocol = IPPROTO_TCP;

    int resolv_addr = getaddrinfo(m_host.data(), "80",
        &m_hostspecs, &m_results);

    if(resolv_addr != 0) {
        LOG_ERROR(std::string(__FUNCTION__), "Unable to resolve address info");
        return false;
    }

    // TODO : Make sure everything gets correct value.
    return true;
}
}
