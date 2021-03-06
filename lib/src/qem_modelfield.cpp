#include "Qem/qem_modelfield.h"
#include "Qem/qem_assert.h"

#include <cstdlib>
#include <iostream>

namespace Qem
{

    ModelId::ModelId(): m_id(QEM_MODELID_INVALID)
	{
	}
	ModelId::ModelId(const unsigned long & id): m_id(id)
	{
	}
	//-----------------------------------------------------------------------------

	const unsigned long ModelId::random(const unsigned long & base)
	{
		unsigned long r = 0;
		for (unsigned int i = 0; i < sizeof(unsigned long)/sizeof(int); i++)
		{
			r = r << (sizeof(int) * 7);
			r |= rand() + (base * rand());
			#ifdef WIN32
			r *= base;
			#endif
		}
		return r;
	}
	//-----------------------------------------------------------------------------
	bool ModelId::isValid() const
	{
        return m_id != QEM_MODELID_INVALID;
	}

	bool ModelId::operator<(const ModelId & second) const
	{
		return (m_id < second.m_id);
	}

	bool ModelId::operator>(const ModelId & second) const
	{
		return (m_id > second.m_id);
	}

	bool ModelId::operator==(const ModelId & second) const
	{
		return (m_id == second.m_id);
	}

    bool ModelId::operator!=(const ModelId & second) const
    {
        return (m_id != second.m_id);
    }

}

