#include "Qem/qem_assert.h"

namespace Qem
{

	void handleAssert(const char* const expr, const char * message, const char * file, const int line)
	{
		throw AssertException(expr, message, file, line);
	}

}
