#ifndef QEM_ASSERT_H
#define QEM_ASSERT_H

#define QEM_ASSERT(X, M)	if(!(X)) { Qem::handleAssert(#X, M, __FILE__, __LINE__); }

#include <Qem/qem.h>
#include <sstream>

namespace Qem
{

    QEM_EXPORT void handleAssert(const char* expr, const char * message, const char * file, const int line);

	class QEM_EXPORT AssertException : public std::exception
	{
		public:
			AssertException(const char* expr, const char * message, const char * file, const int line)
				: expr(expr), message(message), file(file), line(line)
			{
				m_what = "AssertException thrown : " + this->message + " in " + this->file + " at line ";

				std::stringstream ss;
				ss << m_what << this->line;

				m_what = ss.str();
			}

			const char * what() const throw()
			{
				return m_what.c_str();
			}

		private:
			std::string m_what;
			std::string expr;
			std::string message;
			std::string file;
			int line;
	};

}

#endif // QEM_ASSERT_H
