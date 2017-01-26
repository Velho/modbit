#ifndef WEBSTREAM_H
#define WEBSTREAM_H

#include <vector>
#include <functional>

namespace modbit {


///
// Streams the output from Web.
// Fills the buffer with data at each timeout
// interval. Retrieved data is kept in memory so
// no Web data is saved in filesystem.
///
class WebStream {
public:
	explicit WebStream();
	~WebStream();

	// Disallow copying the WebStream.
	WebStream(WebStream&) = delete;
	WebStream &operator=(WebStream&) = delete;

	using BufferEvent = std::function<void()>;


private:
	int m_timeout;
	std::vector<char*> m_buffer;
};

}
#endif
