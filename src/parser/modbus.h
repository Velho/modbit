#ifndef MODBUS_H
#define MODBUS_H

#include "dataparser.h"

namespace modbit {


class Modbus {
public:
	Modbus();
	~Modbus();

	struct Data_t {
		int reg;
		uint16_t val;
	};

	/**
	 * Could be easily sorted out with template if we would
	 * follow the second option, but for now we are going to
	 * represent the data as it is.
	 */
	void AddData(int reg, uint16_t val);

	//std::vector<std::unique_ptr<Data_t>> &GetData() { return m_datatable; }

private:
	std::vector<Data_t*> m_datatable;
};
}

#endif // !MODBUS_H
