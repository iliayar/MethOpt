#pragma once
#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>

#include "function.hpp"

#define SCALE 5
#define PLOT_STEP 0.1

class GraphicsSource : public QObject {
    Q_OBJECT
public:
    GraphicsSource(QObject* parent = nullptr);
signals:
    void draw_point(int x, int y);
    void plot_function(Function*);
};


class TestGraphicsSource : public GraphicsSource {
    Q_OBJECT
public:
    TestGraphicsSource(QObject* parent = nullptr);
// signals:
//     void draw_point(int x, int y);
public slots:
    void timeout();
private:
    QTimer* m_timer;
    double m_x;
};

class Lab1Window : public QMainWindow {
    Q_OBJECT
public:
    explicit Lab1Window(QWidget* parent = nullptr);
    virtual ~Lab1Window();

    void set_source(GraphicsSource*);
public slots:
    void draw_point(int x, int y);
    void plot_function(Function*);
private:
    GraphicsSource* m_source;
    QGraphicsScene* m_scene;
};

