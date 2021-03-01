#include "lab1.hpp"
#include "function.hpp"

#include <QApplication>

Q_DECLARE_METATYPE(Function*)

int main(int argc, char *argv[]) {

    qRegisterMetaType<Function*>();
    
    QApplication app(argc, argv);

    Lab1Window w{};

    w.set_source(new TestGraphicsSource());
    
    w.show();

    return app.exec();
}
