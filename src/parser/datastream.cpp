#include "datastream.h"

#include <string>

namespace modbit {

// Helper macros to shorten some calls.
// Reseting the socket just sets the new one as null which calls the .dtor
#define LOG_ERROR(FUNCTION, USERMSG) \
    m_errorlog.emplace_back(FUNCTION + " :: " + USERMSG + " -> " + std::to_string(WSAGetLastError()));

/* TODO : Perhaps use the Global Data to init the DataStream object. */
/* Global Data */
constexpr size_t g_PACKETSZ { 1024 };


// Underlying class provides high-level implementation
// of stream(without I/O operators for their high abstraction
// cost and Windows specific are stored here.


/**
 * .ctor
 * Initializes the pointer to structure and
 * provides the timeout for reading the output.
 */
DataStream::DataStream(std::string &host) :
    m_host { host }, m_running { true }, m_initsocket { false }
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
    if(!m_initialized) {
        LOG_ERROR(std::string(__FUNCTION__), "Failed to initialize");
        WSACleanup();
        return false;
    }

	CreateSocket();

    return true;
}

void DataStream::Close()
{
    m_running = false;
}

void DataStream::RequestData()
{
    // Run the RequestData loop, providing the data through callbacks.
    while (m_running) {
            Open();
            Write();
            Read();

            Sleep(m_mstimeout);
    }
}

const std::vector<std::string> &DataStream::GetErrorLog()
{
    return m_errorlog;
}

DataStream::StatusCode DataStream::Read()
{
    size_t recv_sz = 1;
    BufferData data{}; // Empty our Buffer.

    // Initializes the nonblocking flag and TCP Socket as Stream.
    //u_long nonblock_flag = 1;
    //ioctlsocket(m_clientsock, FIONBIO, &nonblock_flag);

    size_t buf_len = 1024;

    std::string feed{};
    std::vector<char> buffer(buf_len);
    size_t bytes_read = 0;
    
    do {
        bytes_read = recv(m_clientsock, buffer.data(), buf_len, 0);

        if (bytes_read == -1)
            break;
        else
            feed.append(buffer.begin(), buffer.end());

    } while (bytes_read == buf_len);

    if(recv_sz != -1)
        OnBufferData(std::move(feed));
    
    shutdown(m_clientsock, SD_SEND);

    return StatusCode::OK;
}

bool DataStream::Write()
{
    auto data = GetRequestHeader();
    size_t sent = 0;
    size_t remaining = data.size();
    int result = 0;

    while(remaining > 0) {
        const char *http = data.c_str() + sent;
        result = send(m_clientsock, http, remaining, 0);

        if(result == SOCKET_ERROR) {
            break;
        }

        sent += result;
        remaining -= result;
    }

    // Shutdown the socket but not yet closing as still using it,
    // though if failing to shutdown we have force cleanup.
    if(shutdown(m_clientsock, SD_SEND) == SOCKET_ERROR) {
        LOG_ERROR(std::string(__FUNCTION__), "");
        closesocket(m_clientsock);

        return false;
    }

    return true;
}

std::string DataStream::GetRequestHeader()
{
    std::string http_get;
    http_get += "GET /feed.txt HTTP/1.1\r\n";
    http_get += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n";
    http_get += "Host: tuftuf.gambitlabs.fi\r\n";
    http_get += "Accept: text/html\r\n";
    http_get += "\r\n"; //  End the request with blank line.

    return http_get;
}

bool DataStream::CreateSocket()
{
    // Initializes the required server structures.
    InitServerInfo();

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

    int resolv_addr = getaddrinfo(m_host.c_str(), "80",
        &m_hostspecs, &m_results);

    if(resolv_addr != 0) {
        LOG_ERROR(std::string(__FUNCTION__), "Unable to resolve address info");
        return false;
    }

    // TODO : Make sure everything gets correct value.
    return true;
}
DataStream::StatusCode DataStream::GetResponseStatus(std::string &)
{
    return StatusCode();
}
}
