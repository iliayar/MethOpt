#include <QGraphicsView>
#include <QMainWindow>
#include <iostream>

#include "function.hpp"
#include "lab1.hpp"

Lab1Window::Lab1Window(QWidget* parent)
    : QMainWindow(parent), m_source(nullptr) {
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* graphics_view = new QGraphicsView();

    scene->setSceneRect(-10, -10, 20, 20);
    graphics_view->setScene(scene);
    graphics_view->scale(1 * SCALE, -1 * SCALE);
    graphics_view->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(graphics_view);

    m_scene = scene;
}

void Lab1Window::draw_point(int x, int y) {
    QPen pen{};
    QColor color(Qt::red);
    color.setAlpha(160);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(color);
    QBrush brush{};
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    m_scene->addEllipse(x, y, 0.5, 0.5, pen, brush);

}

void Lab1Window::plot_function(Function* f) {
    QRectF scene_rect = m_scene->sceneRect();
    QPen pen{};
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(Qt::blue);
    pen.setCapStyle(Qt::RoundCap);

    double prev_x = scene_rect.left();
    double prev_y = (*f)(prev_x);
    for (double x = scene_rect.left() + PLOT_STEP; x < scene_rect.right(); x += PLOT_STEP) {
        double new_x = x;
        double new_y = (*f)(new_x);
        m_scene->addLine(prev_x, prev_y, new_x, new_y, pen);
        prev_x = new_x;
        prev_y = new_y;
    }
}

void Lab1Window::set_source(GraphicsSource* source) {
    if (m_source != nullptr) {
        disconnect(m_source, &GraphicsSource::draw_point, this,
                   &Lab1Window::draw_point);
        disconnect(m_source, &GraphicsSource::plot_function, this,
                   &Lab1Window::plot_function);
    }
    m_source = source;
    connect(m_source, &GraphicsSource::draw_point, this,
            &Lab1Window::draw_point);
    connect(m_source, &GraphicsSource::plot_function, this,
            &Lab1Window::plot_function);
}

Lab1Window::~Lab1Window() {}

GraphicsSource::GraphicsSource(QObject* parent) : QObject(parent) {
}

TestGraphicsSource::TestGraphicsSource(QObject* parent)
    : GraphicsSource(parent), m_x(0), m_timer(new QTimer()) {
    connect(m_timer, &QTimer::timeout, this, &TestGraphicsSource::timeout);
    m_timer->start(250);
}

void TestGraphicsSource::timeout() {
    if (m_x < 0.2) {
        emit plot_function(new Parabola());
    }
    emit draw_point(m_x, m_x * m_x);
    m_x += 0.2;
}
