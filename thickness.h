#ifndef THICKNESS_H
#define THICKNESS_H

#include <QDialog>

namespace Ui {
class thickness;
}

class thickness : public QDialog
{
    Q_OBJECT

public:
    explicit thickness(QWidget *parent = nullptr);
    ~thickness();

    qreal change_thickness = 5;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::thickness *ui;
};

#endif // THICKNESS_H
