#include "MainWindow.h"

#include <QApplication>
#include <QTime>

#include "scripting/ScriptEngine.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    MainWindow w;
    w.show();

    ScriptEngine engine;
    engine.installExtensions(QJSEngine::TranslationExtension | QJSEngine::ConsoleExtension);
    engine.globalObject().setProperty("myNumber", 123);
    engine.globalObject().setProperty("myFunction", engine.evaluate("function() {myNumber = 0; return 5;}"));

    const auto &filePath("C:/Users/Fadyl/Desktop/script1.txt");
    const QJSValue &result(engine.evaluateFromFile(filePath));
    if(result.isUndefined()) {
        qDebug() << "Unable to open" << filePath;
        // Normally we should return
    }

    if(result.isError()) {
        qDebug() << "Something goes wrong at line" << result.property("lineNumber").toInt() << ":" << result.toString();
    }

    qDebug() << result.property("zzz").toString();
    qDebug() << engine.globalObject().property("myNumber").toString();
    qDebug() << engine.globalObject().property("myFunction").toString();
    qDebug() << engine.globalObject().property("myFunction").call().toString();
    qDebug() << engine.globalObject().property("myNumber").toString();

    return a.exec();
}
