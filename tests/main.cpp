#include <UnitTest++/UnitTest++.h>

#include <Qem/Qem>

#include "fixtures.h"
#include "watcher.h"

#include <QDebug>

#define QEM_CHECK_ASSERT(Expression) \
			CHECK_THROW(Expression, Qem::AssertException);

#define CHECK_NO_THROW(expression, ExpectedExceptionType) \
	UNITTEST_MULTILINE_MACRO_BEGIN \
		bool caught_ = false; \
		try { expression; } \
	catch (ExpectedExceptionType const& e) { caught_ = true; std::cerr << e.what() << std::endl; } \
		catch (...) {} \
		if (caught_) \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), "Not expected exception: \"" #ExpectedExceptionType "\" thrown"); \
	UNITTEST_MULTILINE_MACRO_END

#define QEM_CHECK_NO_ASSERT(Expression) \
			CHECK_NO_THROW(Expression, Qem::AssertException);

SUITE(QemAgencyManager)
{

	TEST(QemInit)
	{
		Qem::init();
		CHECK(Qem::AgencyManager::empty());
		Qem::shutdown();
	}

	TEST(QemShutdown)
	{
		Qem::init();
		Qem::shutdown();
		CHECK(Qem::AgencyManager::empty());
	}

	TEST(QemAgencyClear)
	{
		CHECK(Qem::AgencyManager::empty());
		Qem::ModelId id = Qem::TestData::createModel();
		CHECK(!Qem::AgencyManager::empty());
		Qem::TestData::destroyModel(id);
		CHECK(Qem::AgencyManager::empty());
	}

}

SUITE(QemModel)
{

	TEST(QemModelCreate)
	{
		Qem::ModelId& id = Qem::TestData::createModel();
		CHECK(id.isValid());
	}

	TEST(QemModelDelete)
	{
		Qem::ModelId& id = Qem::TestData::createModel();
		CHECK(id.isValid());
		Qem::TestData::destroyModel(id);

		QEM_CHECK_ASSERT( Qem::TestData::Reader reader(id) );
	}

	TEST(QemModelId)
	{
		for(unsigned int i = 1; i <= 10000; ++i)
		{
			QEM_CHECK_NO_ASSERT(Qem::TestData::createModel());
		}
	}

}

SUITE(QemReadWrite)
{
	TEST(QemModelReadNotValid)
	{
		Qem::ModelId& id = Qem::TestData::createModel();
		CHECK(id.isValid());

		Qem::TestData::Reader reader(id);

		QEM_CHECK_ASSERT(reader.getId());
	}

	TEST(QemModelReadWriteValid)
	{
		Qem::ModelId& id = Qem::TestData::createModel();
		CHECK(id.isValid());

		Qem::TestData::Writer writer(id);
		writer.setId(42);

		Qem::TestData::Reader reader(id);

		CHECK_EQUAL(reader.getId(), 42);
	}
}

SUITE(QemWatcher)
{

	// TODO

}

int main(int argc, char* argv[])
{
	return UnitTest::RunAllTests();
}
