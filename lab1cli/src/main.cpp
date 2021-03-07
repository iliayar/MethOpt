#include <fire-hpp/fire.hpp>
#include <iostream>
#include <string>

#include "methods.hpp"

void print_data_json(std::vector<IterationData<double>*> data) {
    int i = 0;
    std::cout << "{ \"type\": \"iteration\", \"data\": [ ";
    for (IterationData<double>* d : data) {
        if(i != 0) {
            std::cout << ", ";
        }
        i++;
        switch (d->get_type()) {
            case DataType::Point: {
                auto point = static_cast<IterationPoint<double>*>(d);
                std::cout << "{ \"type\": \"point\", \"x\": " << point->get_x()
                          << ", \"y\": " << point->get_y() << " }";
                break;
            }
            case DataType::Interval: {
                auto point = static_cast<IterationInterval<double>*>(d);
                std::cout << "{ \"type\": \"interval\", \"left\": "
                          << point->get_left_x()
                          << ", \"right\": " << point->get_right_x() << " }";
                break;
            }
            case DataType::Function: {
                auto point = static_cast<IterationFunction<double>*>(d);
                std::cout << "{ \"type\": \"function\" }";
                break;
            }
        }
    }
    std::cout << " ] }" << std::endl;
}

int fired_main(std::string method = fire::arg({"-m", "--method", "Optimization method. Avaliable: dichotomy"}, "dichotomy"),
               double left = fire::arg({"-l", "--left", "Left bound of range"}, -10.0),
               double right = fire::arg({"-r", "--right", "Right bound of range"}, 10.0),
               fire::optional<double> eps = fire::arg({"-e", "--epsilon", "Precision to find minimum til"})) {
    Optimizer<double>* optimzier;
    if (method == "dichotomy") {
        optimzier = new DichotomyMethod<double>(new Parabola<double>(), left,
                                                right, eps.value_or(1e-5));
    }

    if(optimzier == nullptr) {
        std::cerr << "No such method: " << method << std::endl;
        return -1;
    }
    do {
        print_data_json(optimzier->get_data());
    } while (optimzier->forward());
    std::cout << "{ \"type\": \"minimum\", \"x\": "
              << optimzier->get_min().first
              << ", \"y\": " << optimzier->get_min().second << " }"
              << std::endl;
    return 0;
}

FIRE(fired_main)