#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QHash>
#include <QVector>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <QDebug>

#include "utils.h"

QT_BEGIN_NAMESPACE

// transform \r\n into \n
// \r into \n (os9 style)
// backslash-newlines into newlines
static QByteArray cleaned(const QByteArray &input)
{
    QByteArray result;
    result.reserve(input.size());
    const char *data = input;
    char *output = result.data();

    int newlines = 0;
    while (*data) {
        while (*data && is_space(*data))
            ++data;
        bool takeLine = (*data == '#');
        if (*data == '%' && *(data+1) == ':') {
            takeLine = true;
            ++data;
        }
        if (takeLine) {
            *output = '#';
            ++output;
            do ++data; while (*data && is_space(*data));
        }
        while (*data) {
            // handle \\\n, \\\r\n and \\\r
            if (*data == '\\') {
                if (*(data + 1) == '\r') {
                    ++data;
                }
                if (*data && (*(data + 1) == '\n' || (*data) == '\r')) {
                    ++newlines;
                    data += 1;
                    if (*data != '\r')
                        data += 1;
                    continue;
                }
            } else if (*data == '\r' && *(data + 1) == '\n') { // reduce \r\n to \n
                ++data;
            }

            char ch = *data;
            if (ch == '\r') // os9: replace \r with \n
                ch = '\n';
            *output = ch;
            ++output;

            if (*data == '\n') {
                // output additional newlines to keep the correct line-numbering
                // for the lines following the backslash-newline sequence(s)
                while (newlines) {
                    *output = '\n';
                    ++output;
                    --newlines;
                }
                ++data;
                break;
            }
            ++data;
        }
    }
    result.resize(output - result.constData());
    return result;
}

struct QceModel { QString name; QHash<QString, QString> members; };
QByteArray qceExpand(QByteArray & source, const bool & generateMeta)
{
    QString str(QLatin1String(source.data()));
    QRegExp modelExpr("^QEM_MODEL\\(.[A-Z0-9a-z]*\\,.[0-9]{1,1}"); // Matches the model name and members count
    QRegExp modelNameExpr("\\(.[A-Z0-9a-z]*.\\,");
    QRegExp modelEndExpr("\\)");

    QRegExp memberExpr("\\([A-Z0-9a-z]*\\,.*[A-Za-z]*.\\)\\,?");
    QRegExp memberNameExpr("\\([A-Z0-9a-z]*");
    QRegExp memberTypeExpr("[A-Za-z]*.\\)");

    QVector<QceModel*> models;
    QceModel* currentModel = 0;

	QString mocFileBody = "#include <Qem/Qem> \n\n";

	QRegExp metaFieldExpr("^QEM_MODEL_FIELD\\(.*\\,.*\\)"); // Matches the user defined meta types
    QRegExp metaFieldTypeExpr("\\(.*\\,");
    QRegExp metaFieldNameExpr("\\,.*\\)");

    foreach(QString line, str.split("\n"))
    {
        if(modelExpr.indexIn(line) != -1 && currentModel == 0)
        {
            QString line = modelExpr.cap(0);
            if(modelNameExpr.indexIn(line) != -1)
            {
                QString modelName = modelNameExpr.cap(0).remove("(").remove(",").trimmed();
                QceModel* model = new QceModel();
                model->name = modelName;
                models.push_back(model);
                currentModel = model;
            }
        }
        else if(metaFieldExpr.indexIn(line) != -1 && generateMeta)
        {
            QString metaFieldLine = metaFieldExpr.cap(0);

            metaFieldTypeExpr.indexIn(metaFieldLine);
            QString fieldType = metaFieldTypeExpr.cap(0).remove("(").remove(",").trimmed();

            metaFieldNameExpr.indexIn(metaFieldLine);
            QString fieldName = metaFieldNameExpr.cap(0).remove(")").remove(",").trimmed();

			mocFileBody.append(QString("typedef Qem::ModelField< %1 >		m%2;\n").arg(fieldType).arg(fieldName));
        }

        if(currentModel)
        {
            if(memberExpr.indexIn(line) != -1)
            {
                QString member = memberExpr.cap(0);

                memberNameExpr.indexIn(member);
                QString memberName = memberNameExpr.cap(0).remove("(").trimmed();

                memberTypeExpr.indexIn(member);
                QString memberType = memberTypeExpr.cap(0).remove(")").trimmed();

                currentModel->members.insert(memberName, memberType);
            }
            else if(modelEndExpr.indexIn(line) != -1)
            {
                currentModel = 0;
            }
        }
    }

    foreach(QceModel* model, models)
    {
        QString classBody = QString("\nnamespace Qem { \n\tclass %1 : public QObject\n\t{ \n\t\tQ_OBJECT \n\t\tpublic: %1() : QObject() \n\t\t{} \n").arg(model->name);

        QHashIterator<QString,QString> it(model->members);
    classBody.append("\n\tsignals: \n");
        while(it.hasNext())
        {
            it.next();

            classBody.append(QString("\t\t\tvoid on%1Updated( const Qem::ModelId & );\n").arg(it.key()));
        }

        classBody.append("\t};\n} \n");

        mocFileBody.append(classBody).append("\n");
    }

	QString qceFileDef = "QEM_PREPROC_" + ( models.size() ? models.first()->name.toUpper() : "" ) + "_H";
    mocFileBody.prepend(QString("#ifndef %1\n#define %1 \n\n#include <QObject>\n\n").arg(qceFileDef));

    mocFileBody.append(QString("\n\n#endif // %1\n").arg(qceFileDef));

    return mocFileBody.toUtf8();
}

void error(const char *msg = "Invalid argument")
{
    if (msg)
		fprintf(stderr, "qemPreproc: %s\n", msg);
	fprintf(stderr, "Usage: qemPreproc [options] <header-file>\n");
    exit(1);
}
#include <iostream>
int runPreProc(int _argc, char **_argv)
{
	QByteArray fileName;
	QByteArray qemOutputFile;
	bool generateMeta = false;

	QVector<QByteArray> argv;
    for(int i = 1; i < _argc; ++i)
    {
        argv.append( QByteArray::fromRawData(_argv[i], strlen(_argv[i])) );
    }
	int argc = argv.count();

    for (int n = 0; n < argc; ++n)
    {
		QByteArray arg(argv[n]);
        if (arg[0] != '-') {
            if (fileName.isEmpty()) {
                fileName = arg.constData();
                continue;
            }
            error("Too many input files specified");
        }
		QByteArray opt = arg.mid(1);
        switch (opt[0])
        {
            case 'o':
                if (opt[1])
                {
					qemOutputFile = opt.mid(1).split('=').last();
                }
                break;
			case 'm':
				generateMeta = true;
				break;
            case 'h':
            default:
                error();
        }
    }

    if(fileName.isEmpty())
    {
        error("No input file specified");
    }
    else
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray fileContent = file.readAll();
            file.close();

            cleaned(fileContent);
            fileContent = qceExpand(fileContent, generateMeta);

			if(qemOutputFile.isEmpty())
            {
				fprintf(stdout, fileContent.constData());
            }
            else
            {
				QFile outputFile(qemOutputFile);
                if(outputFile.open(QIODevice::WriteOnly))
                {
                    outputFile.write(fileContent);
                    outputFile.close();
                }
                else
					error("Couldn't write to file " + qemOutputFile);
            }
        }
        else
             error("Couldn't open file " + fileName);
    }

    return 0;
}

QT_END_NAMESPACE

int main(int _argc, char **_argv)
{
    return QT_PREPEND_NAMESPACE(runPreProc)(_argc, _argv);
}
