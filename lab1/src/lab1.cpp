#include <QApplication>

#include "function.hpp"
#include "methods.hpp"
#include "lab1ui.hpp"
#include "optimizerUI.hpp"

Q_DECLARE_METATYPE(Function<double> *)

int main(int argc, char *argv[]) {
    qRegisterMetaType<Function<double> *>();
    qRegisterMetaType<Method>();

    QApplication app(argc, argv);

    Lab1Window w{};

    // w.set_source(new OptimizationGraphicsSource<DichotomyMethod<double>,
    // double>(new Parabola<double>(), -40, -5));

    w.show();

    return app.exec();
}
