#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "quickandroid.h"
#include "qadrawableprovider.h"
#include "qasystemdispatcher.h"

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
    Q_UNUSED(vm);
    qDebug("NativeInterface::JNI_OnLoad()");
    QASystemDispatcher::registerNatives();
    return JNI_VERSION_1_6;
}
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;

    /* QuickAndroid Initialization */

    view.engine()->addImportPath("qrc:///"); // Add QuickAndroid into the import path
    QuickAndroid::registerTypes(); // It must be called before loaded any scene
    /* End of QuickAndroid Initialization */

    // Extra features:
    QADrawableProvider* provider = new QADrawableProvider();
    provider->setBasePath("qrc:///");
    view.engine()->addImageProvider("drawable",provider);

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:///main.qml")));
    view.show();

    QVariantMap message;
    message["field1"] = "value1";
    message["field2"] = 10;
    message["field3"] = true;
    QASystemDispatcher::instance()->sendMessage("ping",message);

    message.clear();
    message["title"] = "Quick Android Example";
    message["message"] = "Hello! It is Quick Android Hello World";
    QASystemDispatcher::instance()->sendMessage("notificationManagerNotify",message);

    view.rootContext()->setContextProperty("SystemMessenger",QASystemDispatcher::instance());

    return app.exec();
}
