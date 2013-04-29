#include "Qem/qem_modelfield.h"
#include "Qem/qem_assert.h"

#include <QtCore/QtGlobal>
#include <cstdlib>

namespace Qem
{

	unsigned long ModelId::m_invalidValue = static_cast<unsigned long>(-1);

	ModelId::ModelId(): m_id(ModelId::m_invalidValue)
	{
	}
	ModelId::ModelId(const unsigned long & id): m_id(id)
	{
		QEM_ASSERT(m_id != m_invalidValue, "ModelId created with invalidValue");
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
		return m_id != ModelId::m_invalidValue;
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

	#ifdef QEM_DEBUG
    // TODO : Find a proper way to debug the ID
	QDebug operator<<(QDebug debug, const Qem::ModelId & id)
	{
		debug << id.m_id;
		return debug;
	}
	#endif

}

