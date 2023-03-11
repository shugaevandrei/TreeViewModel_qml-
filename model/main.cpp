#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "model.h"
#include <QQuickWindow>

int main(int argc, char *argv[])
{

    qputenv("QT_QUICK_BACKEND","software");
    QQuickWindow::setSceneGraphBackend("software");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    TreeModel *model = new TreeModel;
    engine.rootContext()->setContextProperty("treeModel", model);
    const QUrl url("qrc:/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
