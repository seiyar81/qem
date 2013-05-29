#include "Qem/qem.h"
#include "Qem/qem_agency.h"

namespace Qem
{
	void init()
	{
		Qem::AgencyManager::init();
	}

	void shutdown()
	{
		Qem::AgencyManager::shutdown();
	}
}
