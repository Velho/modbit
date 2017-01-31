#include "dataparser.h"

namespace modbit {

constexpr char g_HOSTURL[]{ "tuftuf.gambitlabs.fi" };

DataParser::DataParser() :
    m_stream{ new DataStream(std::string(g_HOSTURL)) },
    m_data{}, m_threadrunning { false }
{
}

DataParser::~DataParser()
{
    if(m_threadrunning)
        m_streamthread.join();
}

void DataParser::Connect()
{
    // Initialize the callback.
    m_stream->OnBufferData = std::bind(&DataParser::HandleData, this, std::placeholders::_1);
    // Start Stream thread.
    if(!m_threadrunning)
        m_streamthread = std::thread(&DataStream::RequestData, m_stream);
    else
        m_threadrunning = true;
}

void DataParser::HandleData(std::string &&data)
{
    // If no data initialized or 
    if(data != m_data || m_data.size() == 0)
        m_data = data;
    else
        return;

    m_storage.emplace_back(new std::string(data));
}

void DataParser::CloseStream()
{
}

}