#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <iostream>

#include "function.hpp"
#include "optimizer.hpp"

#define SCALE 4
#define PLOT_STEP 0.1

template <typename T>
void set_color(T* item, QColor color, int width = 1, int alpha = 255) {
    QPen pen{};
    color.setAlpha(alpha);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(width);
    pen.setColor(color);
    pen.setCapStyle(Qt::RoundCap);
    item->setPen(pen);
}

template <typename T>
void set_brush(T* item, QColor color, int alpha = 255) {
    QBrush brush{};
    color.setAlpha(alpha);
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    item->setBrush(brush);
}

QRectF scale_rect(QRectF, double);

class GraphicsSource : public QObject {
    Q_OBJECT
public:
    GraphicsSource(QObject* parent = nullptr);
    void init(QGraphicsScene* scene) {
        m_scene_rect = scale_rect(scene->sceneRect(), SCALE);
        std::cout << m_scene_rect.left() << " " << m_scene_rect.top() << " " << m_scene_rect.width() << " " << m_scene_rect.height() << std::endl;
        init_imp();
    }
protected:
    virtual void init_imp() = 0;

    template<typename T>
    void draw_iteration(std::vector<IterationData<T>*> data) {
        for(QGraphicsItem* item : m_prev_items) {
            emit remove_item(item);
            delete item;
            // QPainter* painter = new QPainter();
            // set_color(painter, Qt::gray);
            // item->paint(painter, new QStyleOptionGraphicsItem());
        }
        m_prev_items.clear();
        for(IterationData<T>* d : data) {
            QGraphicsItem* item;
            switch(d->get_type()) {
            case DataType::Function:
                item = create_iter_function(static_cast<IterationFunction<T>*>(d));
                break;
            case DataType::Point:
                item = create_iter_point(static_cast<IterationPoint<T>*>(d));
                break;
            case DataType::Interval:
                item = create_iter_interval(static_cast<IterationInterval<T>*>(d));
                break;
            }
            m_prev_items.push_back(item);
            emit add_item(item);
        }
    }

    QGraphicsItem* create_function(Function<double>*);
    QGraphicsItem* create_point(double, double);
    QGraphicsItem* create_interval(double, double);

private:
    template <typename T>
    QGraphicsItem* create_iter_point(IterationPoint<T>* point) {
        return create_point(static_cast<double>(point->get_x()),
                            static_cast<double>(point->get_y()));
    }

    template <typename T>
    QGraphicsItem* create_iter_function(IterationFunction<T>* function) {
        return create_function(function->get_function()->to_double());
    }

    template <typename T>
    QGraphicsItem* create_iter_interval(IterationInterval<T>* interval) {
        return create_interval(static_cast<double>(interval->get_left_x()),
                               static_cast<double>(interval->get_right_x()));
    }
signals:
    void add_item(QGraphicsItem*);
    void remove_item(QGraphicsItem*);
protected:
    QRectF m_scene_rect;
    std::vector<QGraphicsItem*> m_prev_items;
};


class TestGraphicsSource : public GraphicsSource {
    Q_OBJECT
public:
    TestGraphicsSource(QObject* parent = nullptr);
    void init_imp();
public slots:
    void timeout();
private:
    QTimer* m_timer;
    double m_x;
    Parabola<double> m_function;
};

class Lab1Window : public QMainWindow {
    Q_OBJECT
public:
    explicit Lab1Window(QWidget* parent = nullptr);
    virtual ~Lab1Window();

    void set_source(GraphicsSource*);
public slots:
    void add_item(QGraphicsItem*);
    void remove_item(QGraphicsItem*);
private:
    GraphicsSource* m_source;
    QGraphicsScene* m_scene;
};

