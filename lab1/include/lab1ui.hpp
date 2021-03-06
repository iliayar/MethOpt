#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPushButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <iostream>

#include "function.hpp"
#include "optimizer.hpp"
#include "methods.hpp"

Q_DECLARE_METATYPE(Method)

#define SCALE 4
#define PLOT_STEP 0.1
#define ITERATION_INTERVAL 500

template <typename T>
void set_pen(T* item, QColor color, int width = 1, int alpha = 255) {
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
    GraphicsSource(QObject* parent = nullptr)
        : QObject(parent), m_auto_iteration(false), m_timer() {}
    void init(QGraphicsScene* scene) {
        m_scene_rect = scale_rect(scene->sceneRect(), SCALE);
        std::cout << m_scene_rect.left() << " " << m_scene_rect.top() << " " << m_scene_rect.width() << " " << m_scene_rect.height() << std::endl;
        connect(&m_timer, &QTimer::timeout, this, &GraphicsSource::timeout);
        init_imp();
    }

    virtual void next_iteration() = 0;

public slots:
    void start_auto_iteration() {
        m_timer.start(ITERATION_INTERVAL);
    }
    void stop_auto_iteration() {
        m_timer.stop();
    }
    void timeout() {
        next_iteration();
    }
protected:
    virtual void init_imp() = 0;

    template<typename T>
    void draw_iteration(std::vector<IterationData<T>*> data) {
        for(QGraphicsItem* item : m_prev_items) {
            emit remove_item(item);
            delete item;
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
    bool m_auto_iteration;
    QTimer m_timer;
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
    void choose_method();
private:
    GraphicsSource* m_source;
    QGraphicsScene* m_scene;
    QPushButton* m_next_button;
    QPushButton* m_start_button;
    QPushButton* m_stop_button;
    QDoubleSpinBox* m_left_spin;
    QDoubleSpinBox* m_right_spin;
    QComboBox* m_methods_combo;

};
