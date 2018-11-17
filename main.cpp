#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "compareresults.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	resultXmlCompare::compareResults compRes;

	engine.rootContext()->setContextProperty("compareResults", &compRes);

	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
