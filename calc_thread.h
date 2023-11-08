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
        CalcThread(SceneModifier* par,
                ratio_model<ratio_susp_data>& model,
                std::shared_ptr<ratio_susp_data>& data) : QThread(par)
    , md(model)
    , dt(data)
    {}

        ~CalcThread() = default;

        void run() override;

    private:
        ratio_model<ratio_susp_data>& md;
        std::shared_ptr<ratio_susp_data>& dt;

signals:
        void resultReady(const tsizes&);
        void finalResult(const tsizes&);
};

#endif
