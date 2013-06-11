#ifndef TESTWATCHER_H
#define TESTWATCHER_H

#include <QObject>
#include <QDebug>

#include "models.h"

class TestWatcher : public QObject
{
	Q_OBJECT
public:
	TestWatcher() : QObject()
	{}

public slots:

	void onTestDataIdUpdated(const Qem::ModelId & id)
	{
		Qem::TestData::Reader reader(id);
		qDebug() << "TestWatcher::onTestDataIdUpdated " << reader.getId();
	}

	void onTestDataBIdUpdated(const Qem::ModelId & id)
	{
		Qem::TestDataB::Reader reader(id);
		qDebug() << "TestWatcher::onTestDataBIdUpdated " << reader.getId();
	}

};


#endif //TESTWATCHER_H
