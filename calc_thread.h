#ifndef _CALC_THREAD_
#define _CALC_THREAD_

#include <QtCore/QObject>
#include <QtCore/QThread>
#include "ratio_mag_model.hpp"
#include "simple_mag_model.hpp"
#include "scenemodifier.h"

class CalcThread : public QThread
{
    Q_OBJECT
    public:
        CalcThread(SceneModifier* par) : QThread(par) {}
        ~CalcThread() = default;

        using Result = tsizes;
        void run() override;

signals:
        void resultReady(const tsizes&);
        void finalResult(const tsizes&);
};

#endif
