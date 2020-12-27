#include "lr_drawer.h"
#include "ui_lr_drawer.h"

LR_drawer::LR_drawer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LR_drawer)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(scene);

    scene->addItem(x_axis);

    scene->addItem(y_axis);

    x_pix2data = x_axis->Pixels2Data();
    y_pix2data = y_axis->Pixels2Data();

}

LR_drawer::~LR_drawer()
{
    delete ui;

}

void LR_drawer::contextMenuEvent(QContextMenuEvent *event)
{
    //called when right-clicking the widget
    QMenu menu(this);
    menu.addAction(ui->actionLoad_file);
    menu.addAction(ui->actionChange_color);
    menu.addAction(ui->actionChange_tickness);
    menu.exec(event->globalPos());
}

void LR_drawer::LinearSpace(double start, double end, QVector<double> &data)
{
    int numOfValues = 500;
    double dl = (end-start)/(double)numOfValues;
    for (int i = 0; i < 500; i++){
        data.push_back(start+i*dl);
    }
}


void LR_drawer::on_actionLoad_file_triggered()
{
    //called when pressing Load input button
    QString filename = QFileDialog::getOpenFileName(this, tr("Load file"));

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();

        QStringList input_list = line.split(QLatin1Char(','), Qt::SkipEmptyParts);
        //convert QString into double, and push it back to vectors
        x.push_back(input_list[0].toDouble());
        y.push_back(input_list[1].toDouble());

        //qDebug()<<line<<Qt::endl;
    }
    file.close();

    for (double &i: x){
        QString x_element = QString::number(i);
        if (i == x.last()) ui->x_input->insert(x_element);
        else ui->x_input->insert(x_element + ',');

    }

    for (double &i: y){
        QString y_element = QString::number(i);
        if (i == y.last()) ui->y_input->insert(y_element);
        else ui->y_input->insert(y_element + ',');
    }

}



void LR_drawer::on_load_clicked()
{
    QBrush brush(Qt::black);

    QPen pen(Qt::black);

    brush.setStyle(Qt::SolidPattern);

    pen.setBrush(brush);

    pen.setWidth(1);

    x_input_list = ui->x_input->text().split(QLatin1Char(','), Qt::SkipEmptyParts);
    y_input_list = ui->y_input->text().split(QLatin1Char(','), Qt::SkipEmptyParts);

    for (int i = 0; i< x_input_list.length(); i++){
        qreal x_coord = x_input_list[i].toDouble();
        qreal y_coord = y_input_list[i].toDouble();
        scene->addEllipse(x_coord*pix_d -2.5, -1*y_coord*pix_d-2.5, 5, 5, pen, brush);

    }
}

void LR_drawer::plotCurve()
{
    QBrush brush(*Line_Color);
    QPen new_pen(brush,local_thickness,Qt::SolidLine);
    for (int i=1; i<user_x_data.size(); i++){
        lineItem = new QGraphicsLineItem(x_pix2data * user_x_data[i-1], -1 * y_pix2data * user_y_data[i], x_pix2data * user_x_data[i],-1 * y_pix2data * user_y_data[i]);
        lineItem->setPen(new_pen);
        curve->addToGroup(lineItem);
    }

    //plot function string
    qreal x_0 = ui->x0_choice->value();
    if (ui->user_mode_choice->currentIndex() == 0){
        QString c0_str = QString::number(c0);

        QString c1_str = QString::number(c1);

        QString func_text = "y = "+c0_str+" + "+c1_str+"x";

        font.setPixelSize(20);
        font.setBold(false);
        font.setFamily("Calibri");

        path.addText(300, -200, font,  func_text);
    }
    else{
        QString c0_str = QString::number(c0);

        QString c1_str = QString::number(c1);

        QString func_text_first = "y = "+c0_str+" + "+c1_str+"x"+"      " + "x <= "+ QString::number(x_0);


        QString c0_2_str = QString::number(c0_2);

        QString c1_2_str = QString::number(c1_2);

        QString func_text_second = "y = "+c0_2_str+" + "+c1_2_str+"x" + +"     " + "x > "+ QString::number(x_0);

        font.setPixelSize(20);
        font.setBold(false);
        font.setFamily("Calibri");

        path.addText(280, -250, font,  func_text_first);
        path.addText(280, -200, font,  func_text_second);
    }
    scene->addPath(path, *Line_Color, QBrush(*Line_Color));

}

void LR_drawer::on_Plot_clicked()
{
    if (ui->user_mode_choice->currentIndex() == 0){
    //when user choose linear mode:
        int index = 0;
        int size = 1; // Size of the array at a given iteration


        x_dynArray = new double[size]; // Setting the size of the array to 1
        y_dynArray = new double[size];

        while (index < x_input_list.length()){

            x_dynArray[index] = x_input_list[index].toDouble();
            y_dynArray[index] = y_input_list[index].toDouble();

            size++;
            index++;

            x_tempArray = new double[size]; // Allocating (size + 1) for extension
            y_tempArray = new double[size];

            for (int j = 0; j < size - 1; j++) {
                x_tempArray[j] = x_dynArray[j];
                y_tempArray[j] = y_dynArray[j];
            }
            // Delete the dynamic array and rename the temporary array to be the new dynamic array

            delete[] x_dynArray;
            x_dynArray = nullptr;
            x_dynArray = x_tempArray;
            x_tempArray = nullptr;

            delete[] y_dynArray;
            y_dynArray = nullptr;
            y_dynArray = y_tempArray;
            y_tempArray = nullptr;
        }
        n = x_input_list.length();

        gsl_fit_linear (x_dynArray, 1, y_dynArray, 1, n, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
        //plot the curve: y = c0 + c1 * x
        start = x_dynArray[0];
        end = x_dynArray[n-1];

        LinearSpace(start, end, user_x_data);
        for(int i = 0; i<user_x_data.size(); i++){
            qreal y_value = c0 + c1 * user_x_data[i];
            user_y_data.push_back(y_value);
        }

        plotCurve();
        scene->addItem(curve);
    }
    else{
    //when user choose bilinear mode:
        qreal x0_threshold = ui->x0_choice->value();

        int index = 0;
         // Size of the arrays at a given iteration
        int index1 = 0;
        int index2 = 0;
        int first_size = 1;
        int second_size = 1;

        x_dynArray = new double[first_size]; // Setting the size of the array to 1
        x_dynArray2 = new double[second_size];

        y_dynArray = new double[first_size];
        y_dynArray2 = new double[second_size];

        while (index < x_input_list.length()){
            if (x_input_list[index].toDouble()<= x0_threshold){
                x_dynArray[index1] = x_input_list[index].toDouble();
                y_dynArray[index1] = y_input_list[index].toDouble();

                index++;
                index1++;
                first_size++;
                x_tempArray = new double[first_size]; // Allocating (size + 1) for extension
                y_tempArray = new double[first_size];

                for (int j = 0; j < first_size - 1; j++) {
                    x_tempArray[j] = x_dynArray[j];
                    y_tempArray[j] = y_dynArray[j];
                }
                // Delete the dynamic array and rename the temporary array to be the new dynamic array

                delete[] x_dynArray;
                x_dynArray = nullptr;
                x_dynArray = x_tempArray;
                x_tempArray = nullptr;

                delete[] y_dynArray;
                y_dynArray = nullptr;
                y_dynArray = y_tempArray;
                y_tempArray = nullptr;
             }
        else{
                x_dynArray2[index2] = x_input_list[index].toDouble();
                y_dynArray2[index2] = y_input_list[index].toDouble();

                index++;
                index2++;
                second_size ++;

                x_tempArray = new double[second_size]; // Allocating (size + 1) for extension
                y_tempArray = new double[second_size];

                for (int j = 0; j < second_size - 1; j++) {
                    x_tempArray[j] = x_dynArray2[j];
                    y_tempArray[j] = y_dynArray2[j];
                }
                // Delete the dynamic array and rename the temporary array to be the new dynamic array

                delete[] x_dynArray2;
                x_dynArray2 = nullptr;
                x_dynArray2 = x_tempArray;
                x_tempArray = nullptr;

                delete[] y_dynArray2;
                y_dynArray2 = nullptr;
                y_dynArray2 = y_tempArray;
                y_tempArray = nullptr;
        }

    }
    gsl_fit_linear (x_dynArray, 1, y_dynArray, 1, index1, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
    //plot the curve: y = c0 + c1 * x

    gsl_fit_linear (x_dynArray2, 1, y_dynArray2, 1, index2, &c0_2, &c1_2, &cov00_2, &cov01_2, &cov11_2, &sumsq_2);

    start = x_dynArray[0];
    end = x_dynArray2[index2-1];
    LinearSpace(start, end, user_x_data);

    for(int i = 0; i<user_x_data.size(); i++){
        qreal y_value = 0;
        if (user_x_data[i] <= index1-1){
            y_value = c0 + c1 * user_x_data[i];
        }
        else{
            y_value = c0_2 + c1_2 * user_x_data[i];

        }
        user_y_data.push_back(y_value);
    }
    plotCurve();
    scene->addItem(curve);

    //debugers
      /*
    for (int i = 0; i < index1; i++){
        qDebug()<<x_dynArray[i]<<", "<< y_dynArray[i]<<Qt::endl;

    }
    for (int i = 0; i < index2; i++){
        qDebug()<<x_dynArray2[i]<<", "<< y_dynArray2[i]<<Qt::endl;

    }

    for (int i = 0; i <user_y_data.size(); i++){
        qDebug()<<user_x_data[i]<<", "<<user_y_data[i]<<Qt::endl;
       }

    qDebug()<<index1<<Qt::endl;
    qDebug()<<index2<<Qt::endl;

    qDebug()<<c0<<Qt::endl;
    qDebug()<<c1<<Qt::endl;
    */
    }
}


void LR_drawer::on_actionChange_color_triggered()
{// called when user changes color
    *Line_Color =QColorDialog::getColor();
   QList<QGraphicsItem *> childList = curve->childItems();
    for (int i = 0; i <childList.size(); i++){
        scene->removeItem(childList[i]);
    }
    path.clear();
    plotCurve();
}



void LR_drawer::on_actionChange_tickness_triggered()
{//called when user changes tickness
    thickness Thickness(this);
    Thickness.exec();

    local_thickness = Thickness.change_thickness;
    QList<QGraphicsItem *> childList = curve->childItems();
    for (int i = 0; i <childList.size(); i++){
         scene->removeItem(childList[i]);
    }
    plotCurve();

}
