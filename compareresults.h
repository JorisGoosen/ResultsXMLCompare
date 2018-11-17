#ifndef COMPARERESULTS_H
#define COMPARERESULTS_H

#include <QObject>
#include <string>
#include <sstream>
#include "resultscomparetable.h"

namespace resultXmlCompare
{

class compareResults : public QObject
{
	Q_OBJECT
public:
	explicit compareResults(QObject *parent = nullptr) : QObject(parent) { }

	bool	compare(std::string resultOld, std::string resultNew, std::stringstream & messages);

	result	convertXmltoResultStruct(QString resultXml, std::stringstream & msgs);

public slots:
	QString compare(QString resultOld, QString resultNew);
};

}

#endif // COMPARERESULTS_H
