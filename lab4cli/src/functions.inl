functions.push_back(new_quad_func({{2, -1}, {-1, 1}}, {2, -3}, 10));
functions.push_back(
        new ordinary_function(
            [](double x, double y) -> double { return x*x*y + 2*x*x + 3*y*y; },
            [](double x, double y) -> std::vector<double> {
                return {2*x*y + 4*x, x*x + 6*y};
            },
            [](double x, double y) -> std::vector<std::vector<double>> {
                return {{2*y + 4, 2*x}, {2*x, 6}};
            }));
functions.push_back(
        new ordinary_function(
            [](double x, double y) -> double { return exp(x*x*y/10); },
            [](double x, double y) -> std::vector<double> {
                return {x*y*exp(x*x*y/10)/5, x*x*exp(x*x*y/10)/10};
            },
            [](double x, double y) -> std::vector<std::vector<double>> {
                return {{x*x*y*y*exp(x*x*y/10)/25 + y*exp(x*x*y/10)/5, x*x*x*y*exp(x*x*y/10)/50 + x*exp(x*x*y/10)/5}, {x*x*x*y*exp(x*x*y/10)/50 + x*exp(x*x*y/10)/5, x*x*x*x*exp(x*x*y/10)/100}};
            }));
