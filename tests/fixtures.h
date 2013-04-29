#ifndef TESTDATA_H
#define TESTDATA_H

#include <Qem/Qem>

#include <QObject>

// QEM model fields
QEM_MODEL_FIELD(int, int)
QEM_MODEL_FIELD(unsigned long, ulong)
QEM_MODEL_FIELD(std::string, string)
QEM_MODEL_FIELD(bool, bool)

QEM_MODEL_FIELD(QString, qString)

QEM_MODEL_FIELD(std::vector<int>, vectorInt)

QEM_MODEL(TestData, 3,
	(Id,		int),
	(Name,		qString),
	(BirthDate,	ulong)
)

QEM_MODEL(TestDataB, 3,
	(Id,		int),
    (Name,		qString),
    (Notes,     vectorInt)
)

#endif // TESTDATA_H
