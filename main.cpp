#include "scenemodifier.h"
#include "magsusp3d.h"

#include <QGuiApplication>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

#include <Qt3DInput/QInputAspect>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
//#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <QOrbitCameraController>
#include <QSlider>
#include <QLineEdit>
#include <QLabel>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    auto* root = new Qt3DCore::QEntity();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0xbfbfbf)));
    QWidget* widget = new QWidget;
    QWidget* container = QWidget::createWindowContainer(view, widget);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    QHBoxLayout *hLayout = new QHBoxLayout();
    widget->setLayout(hLayout);
    hLayout->addWidget(container); // this brings to crash on exit

    widget->setWindowTitle(QStringLiteral("Mag suspension"));

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera
    Qt3DRender::QCamera *cameraEntity = view->camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(-12.0f, 5.0f, 12.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    auto *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1.0f);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(20.0f, 40.0f, 20.0f));
    lightEntity->addComponent(lightTransform);

    Qt3DCore::QEntity *lightEntity1 = new Qt3DCore::QEntity(rootEntity);
    auto *light1 = new Qt3DRender::QPointLight(lightEntity1);
    light1->setColor("white");
    light1->setIntensity(1.0f);
    lightEntity1->addComponent(light1);
    Qt3DCore::QTransform *lightTransform1 = new Qt3DCore::QTransform(lightEntity1);
    lightTransform1->setTranslation(QVector3D(-20.0f, 40.0f, 20.0f));
    lightEntity1->addComponent(lightTransform1);

    // For camera controls
    auto *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    // Scenemodifier
    SceneModifier *modifier = new SceneModifier(rootEntity, widget);
    hLayout->addLayout(modifier->getControlLayout());
    view->setRootEntity(rootEntity);

    // Show window
    widget->show();
    widget->setFixedSize(1000, 500);

    return app.exec();
}
