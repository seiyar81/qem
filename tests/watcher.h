#ifndef TESTWATCHER_H
#define TESTWATCHER_H

#include <QObject>
#include <QDebug>

#include "fixtures.h"

class TestWatcher : public QObject
{
	Q_OBJECT
public:
	TestWatcher() : QObject()
	{}

public slots:

	void onIdUpdated(const Qem::ModelId & id)
	{
		Qem::TestData::Reader reader(id);
		qDebug() << "TestWatcher::onIdUpdated " << reader.getId();
	}

};


#endif //TESTWATCHER_H
