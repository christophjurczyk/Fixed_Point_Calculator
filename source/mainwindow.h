#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define version_string "v1.1"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void initGUI();
    void SetRangeMode();
    void checkRange();
    void getFixedPoint();
    void on_checkBox_RangeAuto_stateChanged();
    void on_checkBox_signed_stateChanged();
    void on_lineEdit_minRange_textChanged(const QString &arg1);
    void on_lineEdit_maxRange_textChanged();
    void on_lineEdit_InputValue_textChanged();
    void on_comboBox_size_currentTextChanged(const QString &arg1);
};




#endif // MAINWINDOW_H
