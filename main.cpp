#include "scenemodifier.h"

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
#include <QLabel>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0xbfbfbf)));
    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    widget->setWindowTitle(QStringLiteral("Mag suspension"));

//    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
//    view->registerAspect(input);

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
    SceneModifier *modifier = new SceneModifier(rootEntity);
    (void)modifier;

    // Set root object of the scene
    view->setRootEntity(rootEntity);

#if 0
    auto *gapLayout = new QHBoxLayout;
    auto *gapText = new QLabel("Gap ");
    gapLayout->addWidget(gapText);

    auto *gapSlider = new QSlider(widget);
    gapSlider->setOrientation(Qt::Horizontal);
    gapSlider->setMinimum(0); //mm
    gapSlider->setMaximum(100); //mm
    gapSlider->setSingleStep(5); //mm
    gapLayout->addWidget(gapSlider);
    QObject::connect(gapSlider, &QAbstractSlider::valueChanged,
            modifier, &SceneModifier::gapValChanged);
    gapSlider->setValue(10);

    vLayout->addLayout(gapLayout);

    // Create control widgets
    auto *slLayout = new QHBoxLayout;
    auto *slText = new QLabel("Anchor ");
    slLayout->addWidget(slText);

    auto *anchorSlider = new QSlider(widget);
    anchorSlider->setOrientation(Qt::Horizontal);
    anchorSlider->setMinimum(10); //mm
    anchorSlider->setMaximum(200); //mm
    anchorSlider->setSingleStep(5); //mm
    slLayout->addWidget(anchorSlider);
    QObject::connect(anchorSlider, &QAbstractSlider::valueChanged,
            modifier, &SceneModifier::anchorValChanged);
    anchorSlider->setValue(20);

    vLayout->addLayout(slLayout);

    auto *aslLayout = new QHBoxLayout;
    auto *aslText = new QLabel("a_m");
    aslLayout->addWidget(aslText);
    auto *aSlider = new QSlider(widget);
    aSlider->setOrientation(Qt::Horizontal);
    aSlider->setMinimum(10); //mm
    aSlider->setMaximum(300); //mm
    aSlider->setSingleStep(5); //mm
    aslLayout->addWidget(aSlider);
    QObject::connect(aSlider, &QAbstractSlider::valueChanged,
            modifier, &SceneModifier::aValChanged);
    aSlider->setValue(150);

    vLayout->addLayout(aslLayout);

    auto *bslLayout = new QHBoxLayout;
    auto *bslText = new QLabel("b_m");
    bslLayout->addWidget(bslText);
    auto *bSlider = new QSlider(widget);
    bSlider->setOrientation(Qt::Horizontal);
    bSlider->setMinimum(10); //mm
    bSlider->setMaximum(300); //mm
    bSlider->setSingleStep(5); //mm
    bslLayout->addWidget(bSlider);
    QObject::connect(bSlider, &QAbstractSlider::valueChanged,
            modifier, &SceneModifier::bValChanged);
    bSlider->setValue(150);

    vLayout->addLayout(bslLayout);

    auto *cLayout = new QHBoxLayout;
    auto *cText = new QLabel("c");
    cLayout->addWidget(cText);
    auto *cSlider = new QSlider(widget);
    cSlider->setOrientation(Qt::Horizontal);
    cSlider->setMinimum(10); //mm
    cSlider->setMaximum(300); //mm
    cSlider->setSingleStep(5); //mm
    cLayout->addWidget(cSlider);
    QObject::connect(cSlider, &QAbstractSlider::valueChanged,
            modifier, &SceneModifier::cValChanged);
    cSlider->setValue(150);
    vLayout->addLayout(cLayout);

    auto *hhLayout = new QHBoxLayout;
    auto *hText = new QLabel("h");
    hhLayout->addWidget(hText);
    auto *hSlider = new QSlider(widget);
    hSlider->setOrientation(Qt::Horizontal);
    hSlider->setMinimum(100); //mm
    hSlider->setMaximum(500); //mm
    hSlider->setSingleStep(5); //mm
    hhLayout->addWidget(hSlider);
    QObject::connect(hSlider, &QAbstractSlider::valueChanged,
            modifier, &SceneModifier::hValChanged);
    hSlider->setValue(350);
    vLayout->addLayout(hhLayout);

    auto *lLayout = new QHBoxLayout;
    auto *lText = new QLabel("l");
    lLayout->addWidget(lText);
    auto *lSlider = new QSlider(widget);
    lSlider->setOrientation(Qt::Horizontal);
    lSlider->setMinimum(300); //mm
    lSlider->setMaximum(900); //mm
    lSlider->setSingleStep(5); //mm
    lLayout->addWidget(lSlider);
    QObject::connect(lSlider, &QAbstractSlider::valueChanged,
            modifier, &SceneModifier::lValChanged);
    lSlider->setValue(450);
    vLayout->addLayout(lLayout);
#endif

    QPushButton* stBut = new QPushButton("Start");
    QObject::connect(stBut, &QPushButton::pressed,
            modifier, &SceneModifier::startCalculation);
    vLayout->addWidget(stBut);



    // Show window
    widget->show();
    widget->resize(1200, 800);
//    widget->start();

    return app.exec();
}
