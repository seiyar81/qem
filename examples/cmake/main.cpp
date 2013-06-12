#include "watchers.h"

int main(int argc, char* argv[])
{
	TestWatcher* w = new TestWatcher();

    Qem::init();

	qDebug() << "Qem::init() ok";

	Qem::ModelId& id = Qem::TestAggregator::createModel();

	Qem::TestAggregator::Reader reader( id );
	Qem::TestData::Writer writer1 = reader.getTestDataWriter();
	Qem::TestData::Reader reader1 = reader.getTestDataReader();
	Qem::TestData::Watcher watcher1 = reader.getTestDataWatcher();
	Qem::TestDataB::Writer writer2 = reader.getTestDataBWriter();
	Qem::TestDataB::Reader reader2 = reader.getTestDataBReader();
	Qem::TestDataB::Watcher watcher2 = reader.getTestDataBWatcher();

	watcher1.onIdUpdated(w, SLOT(onTestDataIdUpdated(Qem::ModelId)));
	writer1.setId( 666 );
	qDebug() << "main : " << reader1.getId();

	watcher2.onIdUpdated(w, SLOT(onTestDataBIdUpdated(Qem::ModelId)));
	writer2.setId( 555 );
	qDebug() << "main : " << reader2.getId();

	Qem::TestAggregator::destroyModel( id );

    Qem::shutdown();

	qDebug() << "Qem::shutdown() ok";

	delete w;

    return EXIT_SUCCESS;
}
