#ifndef QEM_ASSERT_H
#define QEM_ASSERT_H

#define QEM_ASSERT(X, M)	if(!(X)) { Qem::handleAssert(#X, M, __FILE__, __LINE__); }

#include <sstream>

namespace Qem
{

	void handleAssert(const char* expr, const char * message, const char * file, const int line);

	class AssertException
	{
		public:
			AssertException(const char* expr, const char * message, const char * file, const int line)
				: expr(expr), message(message), file(file), line(line)
			{
			}

			std::string what() const
			{
				std::string what = "AssertException thrown : " + message + " in " + file + " at line ";

				std::stringstream ss;
				ss << what << line;

				return ss.str();
			}

		private:
			std::string expr;
			std::string message;
			std::string file;
			int line;
	};

}

#endif // QEM_ASSERT_H
