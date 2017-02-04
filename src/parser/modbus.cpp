#include "modbus.h"

namespace modbit {

Modbus::Modbus()
{
}

Modbus::~Modbus()
{
	for(int i = 0; i < m_datatable.size(); i++)
		delete m_datatable[i];
}

void Modbus::AddData(int reg, uint16_t val)
{
	auto ptr = new Data_t;
	ptr->reg = reg;
	ptr->val = val;

	m_datatable.emplace_back(ptr);
}

}