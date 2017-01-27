#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <vector>
#include <functional>

namespace modbit {


///
// Streams the output from Web.
// Fills the buffer with data at each timeout
// interval. Retrieved data is kept in memory so
// no Web data is saved in filesystem.
///
class DataStream {
public:
	explicit DataStream();
	~DataStream();

	// Disallow copying the DataStream.
	DataStream(DataStream&) = delete;
	DataStream &operator=(DataStream&) = delete;

	using BufferEvent = std::function<void()>;

private:
	int m_timeout;
	std::vector<char*> m_buffer;
};

}
#endif
