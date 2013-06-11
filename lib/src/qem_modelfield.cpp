#include "Qem/qem_modelfield.h"
#include "Qem/qem_assert.h"

#include <cstdlib>

namespace Qem
{

    ModelId::ModelId(): m_id(QEM_MODELID_INVALID)
	{
	}
	ModelId::ModelId(const unsigned long & id): m_id(id)
	{
	}
    ModelId::ModelId(const ModelId & id): m_id(id.m_id)
    {
    }
	ModelId::ModelId(ModelId && id)
	{
		m_id = id.m_id;
	}
	//-----------------------------------------------------------------------------
	Qem::ModelId ModelId::random(const unsigned long & base)
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
		return Qem::ModelId(r);
	}
	//-----------------------------------------------------------------------------
	bool ModelId::isValid() const
	{
        return m_id != QEM_MODELID_INVALID;
	}

    ModelId& ModelId::operator=(ModelId& id)
    {
        if(&id != this)
        {
            this->m_id = id.m_id;
        }
        return *this;
    }

	ModelId& ModelId::operator=(ModelId&& id)
	{
		if(&id != this)
		{
			this->m_id = id.m_id;
		}
        return *this;
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

