#include "lab1ui.hpp"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QPushButton>
#include <iostream>

#include "QVariant"
#include "function.hpp"
#include "methods.hpp"
#include "optimizerUI.hpp"

QRectF scale_rect(QRectF rect, double scale) {
    return QRectF(rect.left() * scale, rect.top() * scale, rect.width() * scale,
                  rect.height() * scale);
}

Lab1Window::Lab1Window(QWidget* parent)
    : QMainWindow(parent), m_source(nullptr) {
    QFrame* main_box = new QFrame();
    QVBoxLayout* main_layout = new QVBoxLayout(main_box);

    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* graphics_view = new QGraphicsView();

    QGroupBox* buttons_box = new QGroupBox(tr("&Iteration"));
    QHBoxLayout* buttons_layout = new QHBoxLayout(buttons_box);

    QPushButton* next_button = new QPushButton(tr("&Next iteration"));
    QPushButton* start_button = new QPushButton(tr("&Start auto iteration"));
    QPushButton* stop_button = new QPushButton(tr("&Stop auto iteration"));

    QGroupBox* view_box = new QGroupBox();
    QHBoxLayout* view_layout = new QHBoxLayout();

    QGroupBox* options_box = new QGroupBox(tr("&Options"));
    QVBoxLayout* options_layout = new QVBoxLayout();

    QDoubleSpinBox* left_spin = new QDoubleSpinBox();
    QDoubleSpinBox* right_spin = new QDoubleSpinBox();
    QComboBox* methods_combo = new QComboBox();
    QLabel* range_label = new QLabel(" - ");
    QPushButton* options_button = new QPushButton(tr("&Apply"));

    QGroupBox* range_box = new QGroupBox(tr("&Range"));
    QHBoxLayout* range_layout = new QHBoxLayout();

    buttons_box->setLayout(buttons_layout);

    buttons_layout->QLayout::addWidget(start_button);
    buttons_layout->QLayout::addWidget(next_button);
    buttons_layout->QLayout::addWidget(stop_button);

    main_box->setLayout(main_layout);
    main_layout->addWidget(view_box);
    main_layout->addWidget(buttons_box);

    view_box->setLayout(view_layout);
    options_box->setLayout(options_layout);

    range_box->setLayout(range_layout);

    view_layout->addWidget(graphics_view);
    view_layout->addWidget(options_box);

    options_layout->addWidget(methods_combo);
    options_layout->addWidget(range_box);
    options_layout->addWidget(options_button);

    range_layout->addWidget(left_spin);
    range_layout->addWidget(range_label);
    range_layout->addWidget(right_spin);

    double x = -graphics_view->width() / 2;
    double y = -graphics_view->height() / 2;
    double w = graphics_view->width();
    double h = graphics_view->height();
    QRectF scaled_rect = scale_rect(QRectF(x, y, w, h), 1 / (double)SCALE);
    graphics_view->scale(1 * SCALE, -1 * SCALE);
    scene->setSceneRect(scaled_rect);
    graphics_view->setScene(scene);
    graphics_view->setRenderHint(QPainter::Antialiasing);

    left_spin->setRange(scaled_rect.left(), scaled_rect.right());
    right_spin->setRange(scaled_rect.left(), scaled_rect.right());

    methods_combo->addItem("Метод дихотомии",
                           QVariant::fromValue<Method>(Method::Dichotomy));
    methods_combo->addItem("Метод золотого сечения",
                           QVariant::fromValue<Method>(Method::GoldenSection));
    methods_combo->addItem("Метод Фибоначчи",
                           QVariant::fromValue<Method>(Method::Fibonacci));
    methods_combo->addItem("Метод парабол",
                           QVariant::fromValue<Method>(Method::Parabolas));
    methods_combo->addItem("Метод Брента",
                           QVariant::fromValue<Method>(Method::Brent));

    connect(options_button, &QPushButton::clicked, this,
            &Lab1Window::choose_method);

    setCentralWidget(main_box);

    m_scene = scene;
    m_next_button = next_button;
    m_start_button = start_button;
    m_stop_button = stop_button;
    m_left_spin = left_spin;
    m_right_spin = right_spin;
    m_methods_combo = methods_combo;
}

void Lab1Window::choose_method() {
    double left = m_left_spin->value();
    double right = m_right_spin->value();
    m_scene->clear();
    switch (qvariant_cast<Method>(m_methods_combo->currentData())) {
        case Method::Dichotomy: {
            set_source(
                new OptimizationGraphicsSource<DichotomyMethod<double>, double>(
                    new Var2Function<double>, left, right));
            break;
        }
        case Method::Fibonacci: {
            set_source(
                new OptimizationGraphicsSource<FibonacciMethod<double>, double>(
                    new Var2Function<double>, left, right));
            break;
        }
        case Method::Brent: {
            set_source(
                new OptimizationGraphicsSource<BrentMethod<double>, double>(
                    new Var2Function<double>, left, right));
            break;
        }
        case Method::Parabolas: {
            set_source(
                new OptimizationGraphicsSource<ParabolasMethod<double>, double>(
                    new Var2Function<double>, left, right));
            break;
        }
        case Method::GoldenSection: {
            set_source(
                new OptimizationGraphicsSource<GoldenSectionMethod<double>,
                                               double>(new Var2Function<double>,
                                                       left, right));
            break;
        }
    }
}

void Lab1Window::add_item(QGraphicsItem* item) { m_scene->addItem(item); }

void Lab1Window::remove_item(QGraphicsItem* item) { m_scene->removeItem(item); }

void Lab1Window::set_source(GraphicsSource* source) {
    if (m_source != nullptr) {
        disconnect(m_source, &GraphicsSource::add_item, this,
                   &Lab1Window::add_item);
        disconnect(m_source, &GraphicsSource::remove_item, this,
                   &Lab1Window::remove_item);
        disconnect(m_next_button, &QPushButton::clicked, m_source,
                   &GraphicsSource::next_iteration);
        disconnect(m_start_button, &QPushButton::clicked, m_source,
                   &GraphicsSource::start_auto_iteration);
        disconnect(m_stop_button, &QPushButton::clicked, m_source,
                   &GraphicsSource::stop_auto_iteration);
    }
    m_source = source;
    connect(m_source, &GraphicsSource::add_item, this, &Lab1Window::add_item);
    connect(m_source, &GraphicsSource::remove_item, this,
            &Lab1Window::remove_item);
    connect(m_next_button, &QPushButton::clicked, m_source,
            &GraphicsSource::next_iteration);
    connect(m_start_button, &QPushButton::clicked, m_source,
            &GraphicsSource::start_auto_iteration);
    connect(m_stop_button, &QPushButton::clicked, m_source,
            &GraphicsSource::stop_auto_iteration);
    m_source->init(m_scene);
}

Lab1Window::~Lab1Window() {}

QGraphicsItem* GraphicsSource::create_point(double x, double y, QColor color) {
    QGraphicsEllipseItem* point = new QGraphicsEllipseItem(x, y, 6.0 / SCALE, 6.0 / SCALE);
    set_pen(point, color);
    set_brush(point, color);
    return point;
}

QGraphicsItem* GraphicsSource::create_function(Function<double>* function, QColor color) {
    QGraphicsItemGroup* group = new QGraphicsItemGroup();

    double prev_x = m_scene_rect.left();
    double prev_y = (*function)(prev_x);
    for (double x = m_scene_rect.left() + PLOT_STEP; x < m_scene_rect.right();
         x += PLOT_STEP) {
        double new_x = x;
        double new_y = (*function)(new_x);
        QGraphicsLineItem* line =
            new QGraphicsLineItem(prev_x, prev_y, new_x, new_y);
        set_pen(line, color);
        group->addToGroup(line);
        prev_x = new_x;
        prev_y = new_y;
    }
    return group;
}

QGraphicsItem* GraphicsSource::create_interval(double left, double right) {
    QGraphicsItemGroup* group = new QGraphicsItemGroup();

    QGraphicsLineItem* left_line = new QGraphicsLineItem(
        left, m_scene_rect.bottom(), left, m_scene_rect.top());
    QGraphicsLineItem* right_line = new QGraphicsLineItem(
        right, m_scene_rect.bottom(), right, m_scene_rect.top());
    set_pen(left_line, Qt::gray, 1, 100);
    set_pen(right_line, Qt::gray, 1, 100);

    group->addToGroup(left_line);
    group->addToGroup(right_line);
    return group;
}
