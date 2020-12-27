#ifndef LR_DRAWER_H
#define LR_DRAWER_H

#include <QMainWindow>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFile>
#include <QFileDialog>
#include <QColorDialog>
#include <QTextStream>
#include <QDebug>
#include <QVector>
#include <gsl/gsl_fit.h>
#include <QGraphicsScene>
#include <qgraphicsaxisitem.h>
#include <QStringList>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>
#include <QPainterPath>
#include <QFont>
#include "thickness.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LR_drawer; }
QT_END_NAMESPACE

class LR_drawer : public QMainWindow
{
    Q_OBJECT

public:
    LR_drawer(QWidget *parent = nullptr);
    ~LR_drawer();
    void contextMenuEvent(QContextMenuEvent *event);
    void LinearSpace (double start, double end, QVector<double>& data);

private slots:
    void on_actionLoad_file_triggered();

    void on_load_clicked();

    void plotCurve();

    void on_Plot_clicked();

    void on_actionChange_color_triggered();

    void on_actionChange_tickness_triggered();

private:
    Ui::LR_drawer *ui;
    QVector<double> x;
    QVector<double> y;
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsAxisItem *x_axis = new QGraphicsAxisItem('x',0,600,10,1,0);
    QGraphicsAxisItem *y_axis = new QGraphicsAxisItem('y',0,600,10,1,0);

    QStringList  x_input_list;
    QStringList  y_input_list;
    qreal pix_d = x_axis->d_pix;

    qreal x_pix2data, y_pix2data;

    //pointers for x and y arrays:
    double* x_dynArray = nullptr; // Declaring the dynamic array
    double* x_tempArray = nullptr; // Declaring the temporary array
    double* x_dynArray2 = nullptr;  //used for bilinear fit mode

    double* y_dynArray = nullptr;
    double* y_tempArray = nullptr;
    double* y_dynArray2 = nullptr;
    //parameters for linear regression:
    double c0, c1, cov00, cov01, cov11, sumsq;
    double c0_2, c1_2, cov00_2, cov01_2, cov11_2, sumsq_2;

    //range of input values
    int n;
    double start;
    double end;


    //data container
    QVector <double> user_x_data, user_y_data;
    QGraphicsLineItem *lineItem = new QGraphicsLineItem();
    QGraphicsItemGroup *curve = new QGraphicsItemGroup();

    //function string:
    QPainterPath path;

    //font and color:
    QFont font;
    QColor *Line_Color = new QColor(Qt::black);

    //tickness:
    qreal local_thickness = 1;
};
#endif // LR_DRAWER_H
