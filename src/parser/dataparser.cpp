#include "modbus.h"

#include <string>
#include <sstream>

namespace modbit {

constexpr char g_HOSTURL[]{ "tuftuf.gambitlabs.fi" };

DataParser::DataParser() :
    m_stream { new DataStream(std::string(g_HOSTURL)) },
    m_parseridx { 0 }, m_threadrunning { false }, m_data { new Modbus }
{
}

DataParser::~DataParser()
{
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

void DataParser::Disconnect()
{
    m_threadrunning = false;
	m_stream->SetRunning(false);
}

Modbus * DataParser::GetData()
{
    return m_data.get();
}

void DataParser::HandleData(std::string &&data)
{
    m_storage.emplace_back(new std::string(data));
    m_storagesz = m_storage.size();

    ParseData();
}

#include <inttypes.h> /* strtoimax */

static bool
str_to_uint16(const char *str, uint16_t *res)
{
    char *end;
    errno = 0;
    intmax_t val = strtoimax(str, &end, 10);
    if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0')
        return false;
    *res = (uint16_t)val;
    return true;
}

void DataParser::ParseData()
{
    std::string &strline = (*m_storage[m_parseridx]);
    // We want to get the header out of the way..

    auto header_sz = strline.find("\r\n\r\n");
    auto body = strline.substr(header_sz, strline.size());

    std::istringstream resp_body(body);
    std::string value;

    bool start = false;
    int idx = 0;

    // Loop over the response body's values.
    // We'll omit the date for now.. 
    while(std::getline(resp_body, value)) {
        // Skip the date and double \r.
        if (!start && idx < 3) {
            idx++;
            continue;
        }
        else
        {
            start = true;
        }

        if (start && !value.empty())
        {
            auto split = value.find(":");
            
            if (split == std::string::npos)
                break;

            int reg = std::stoi(value.substr(0, split));

            // Cast to uint16_t through vector.
            auto str_reg_val = value.substr(split + 1, value.size());
            uint16_t reg_val = 0;

            if (!str_reg_val.empty())
            {
                if (str_to_uint16(str_reg_val.data(), &reg_val)) { // str_to_uint16 doesnt handle zeros.
                    m_data->AddData(reg, reg_val);
                }
            }
        }
    }

    //std::vector<uint16_t> arr((str_reg_val.size() + sizeof(uint16_t) - 1) / sizeof std::uint16_t ); // = new std::vector<uint16_t>
    //std::copy_n(str_reg_val.data(), str_reg_val.size(), reinterpret_cast<char*>(&arr[0])); // -D_SCL_SECURE_NO_WARNINGS
}


void DataParser::CloseStream()
{
}

}