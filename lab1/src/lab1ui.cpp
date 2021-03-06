#include "lab1ui.hpp"

#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QMainWindow>
#include <iostream>

#include "function.hpp"

QRectF scale_rect(QRectF rect, double scale) {
    return QRectF(rect.left() * scale, rect.top() * scale, rect.width() * scale,
                  rect.height() * scale);
}

Lab1Window::Lab1Window(QWidget* parent)
    : QMainWindow(parent), m_source(nullptr) {
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* graphics_view = new QGraphicsView();

    double x = -graphics_view->width() / 2;
    double y = -graphics_view->height() / 2;
    double w = graphics_view->width();
    double h = graphics_view->height();
    scene->setSceneRect(scale_rect(QRectF(x, y, w, h), 1 / (double)SCALE));
    graphics_view->setScene(scene);
    graphics_view->scale(1 * SCALE, -1 * SCALE);
    graphics_view->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(graphics_view);

    m_scene = scene;
}

void Lab1Window::add_item(QGraphicsItem* item) { m_scene->addItem(item); }

void Lab1Window::remove_item(QGraphicsItem* item) { m_scene->removeItem(item); }

void Lab1Window::set_source(GraphicsSource* source) {
    if (m_source != nullptr) {
        disconnect(m_source, &GraphicsSource::add_item, this,
                   &Lab1Window::add_item);
        disconnect(m_source, &GraphicsSource::remove_item, this,
                   &Lab1Window::remove_item);
    }
    m_source = source;
    connect(m_source, &GraphicsSource::add_item, this, &Lab1Window::add_item);
    connect(m_source, &GraphicsSource::remove_item, this,
            &Lab1Window::remove_item);
    m_source->init(m_scene);
}

Lab1Window::~Lab1Window() {}

GraphicsSource::GraphicsSource(QObject* parent) : QObject(parent) {}

QGraphicsItem* GraphicsSource::create_point(double x, double y) {
    QGraphicsEllipseItem* point = new QGraphicsEllipseItem(x, y, 0.5, 0.5);
    set_color(point, Qt::red);
    set_brush(point, Qt::red);
    return point;
}

QGraphicsItem* GraphicsSource::create_function(Function<double>* function) {
    QGraphicsItemGroup* group = new QGraphicsItemGroup();

    double prev_x = m_scene_rect.left();
    double prev_y = (*function)(prev_x);
    for (double x = m_scene_rect.left() + PLOT_STEP; x < m_scene_rect.right();
         x += PLOT_STEP) {
        double new_x = x;
        double new_y = (*function)(new_x);
        QGraphicsLineItem* line =
            new QGraphicsLineItem(prev_x, prev_y, new_x, new_y);
        set_color(line, Qt::blue);
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
    set_color(left_line, Qt::gray, 1, 100);
    set_color(right_line, Qt::gray, 1, 100);

    group->addToGroup(left_line);
    group->addToGroup(right_line);
    return group;
}

void TestGraphicsSource::init_imp() {
    emit add_item(create_function(&m_function));
}

TestGraphicsSource::TestGraphicsSource(QObject* parent)
    : GraphicsSource(parent), m_x(0), m_timer(new QTimer()), m_function() {
    connect(m_timer, &QTimer::timeout, this, &TestGraphicsSource::timeout);
    m_timer->start(250);
}

void TestGraphicsSource::timeout() {
    draw_iteration<double>({new IterationPoint(m_x, m_x * m_x),
                            new IterationInterval(m_x - 1, m_x + 1)});
    m_x += 0.2;
}
