#include "Qem/qem_agency.h"

#include "Qem/qem_assert.h"

namespace Qem
{

	std::set< Qem::ModelId > AgencyManager::m_runningAgencies = std::set< Qem::ModelId >();

	void AgencyManager::init()
	{
		QEM_ASSERT(m_runningAgencies.empty(), "AgencyManager is not empty");
	}

	void AgencyManager::shutdown()
	{
		QEM_ASSERT(m_runningAgencies.empty(), "AgencyManager is not empty");
	}

	bool AgencyManager::empty()
	{
		return (m_runningAgencies.empty());
	}

	void AgencyManager::registerAgency(const Qem::ModelId & id)
	{
		QEM_ASSERT(m_runningAgencies.find(id) == m_runningAgencies.end(), "Agency already registered");
		m_runningAgencies.insert(id);
	}

	void AgencyManager::unregisterAgency(const Qem::ModelId & id)
	{
		QEM_ASSERT(m_runningAgencies.find(id) != m_runningAgencies.end(), "Agency not registered");
		m_runningAgencies.erase(m_runningAgencies.find(id));
	}

}
