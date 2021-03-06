#include "lab1ui.hpp"
#include "function.hpp"

#include <QApplication>

Q_DECLARE_METATYPE(Function<double>*)

int main(int argc, char *argv[]) {

    qRegisterMetaType<Function<double>*>();
    
    QApplication app(argc, argv);

    Lab1Window w{};

    w.set_source(new TestGraphicsSource());
    
    w.show();

    return app.exec();
}
