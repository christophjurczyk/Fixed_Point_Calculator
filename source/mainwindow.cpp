#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fixed_point_calculations.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initGUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------- GUI events ----------
void MainWindow::on_checkBox_RangeAuto_stateChanged()
{
    SetRangeMode();
}
void MainWindow::on_checkBox_signed_stateChanged()
{
    if(ui->checkBox_RangeAuto->checkState())
        SetRangeMode();
    getFixedPoint();
}
void MainWindow::on_lineEdit_minRange_textChanged(const QString &arg1)
{
    if(!ui->checkBox_RangeAuto->checkState()){
        // manual range
        if(arg1.toDouble() > 0){
            ui->lineEdit_minRange->setText("0");
        }
        checkRange();
    }
    getFixedPoint();
}
void MainWindow::on_lineEdit_maxRange_textChanged()
{
    if(!ui->checkBox_RangeAuto->checkState())
        checkRange();
    getFixedPoint();
}
void MainWindow::on_lineEdit_InputValue_textChanged()
{
    SetRangeMode();
    checkRange();
    getFixedPoint();
}
void MainWindow::on_comboBox_size_currentTextChanged(const QString &arg1)
{
    if(arg1.toInt() > 64) ui->comboBox_size->setCurrentText("64");
    getFixedPoint();
}
// --------------------------------

// ---------- GUI update functions ----------
void MainWindow::initGUI(){
    QString windowTitle = "Fixed Point Calculator ";
    windowTitle += version_string;
    this->setWindowTitle(windowTitle);

    ui->label_github->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_github->setOpenExternalLinks(true);

    QRegExp numbers_only("[+-]?([0-9]*[.])?[0-9]+");
    QRegExp pos_numbers_only("[+]?([0-9]*[.])?[0-9]+");
    QRegExp pos_dec_numbers_only("^\\d\\d?$");
    QRegExp neg_numbers_only("[-]?([0-9]*[.])?[0-9]+");
    ui->lineEdit_InputValue->setValidator(new QRegExpValidator(numbers_only, this));
    ui->lineEdit_InputValue->setText("3.14159265359");
    ui->lineEdit_maxRange->setValidator(new QRegExpValidator(pos_numbers_only, this));
    ui->lineEdit_minRange->setValidator(new QRegExpValidator(neg_numbers_only, this));
    ui->comboBox_size->setValidator(new QRegExpValidator(pos_dec_numbers_only, this));
    ui->label_error->setVisible(false);

    SetRangeMode();
    getFixedPoint();
}
void MainWindow::SetRangeMode()
{
    // enable/disable lineEdits
    bool range_mode = ui->checkBox_RangeAuto->checkState(); // true: auto  false: manual
    ui->lineEdit_minRange->setEnabled(!range_mode);
    ui->lineEdit_maxRange->setEnabled(!range_mode);

    // update lineEdits values
    if(range_mode)
    {   // auto range
        ui->checkBox_signed->setEnabled(true);
        range newRange = fxp_getRange(ui->lineEdit_InputValue->text().toDouble(), ui->checkBox_signed->checkState());
        ui->lineEdit_minRange->setText(QString::number(newRange.min));
        ui->lineEdit_maxRange->setText(QString::number(newRange.max));
    }else{
        // manual mode
        ui->checkBox_signed->setEnabled(false);
    }
}

void MainWindow::checkRange()
{
    double input_value = ui->lineEdit_InputValue->text().toDouble();
    double range_min = ui->lineEdit_minRange->text().toDouble();
    double range_max = ui->lineEdit_maxRange->text().toDouble();

    if((input_value < range_min) || (input_value > range_max))
    {
        ui->label_error->setText("Input value out of input range!");
        ui->label_error->setVisible(true);
    }else{
        ui->label_error->setVisible(false);
    }
}

void MainWindow::getFixedPoint()
{
    QString fxp_type_string;
    double input_value = ui->lineEdit_InputValue->text().toDouble();
    int range_min = ui->lineEdit_minRange->text().toInt();
    int range_max = ui->lineEdit_maxRange->text().toInt();
    unsigned int size = ui->comboBox_size->currentText().toUInt();
    fxp_info result = fxp_getFixedPoint(range_min,range_max,input_value,size);

    if(result.out_of_size)
    {
        ui->label_error->setText("Input value out of input size!");
        ui->label_error->setVisible(true);

        fxp_type_string += "int_" + QString::number(result.n) + "." + QString::number(result.k);
        ui->label_type->setText(fxp_type_string);
        ui->lineEdit_fxp_minRange->setText(QString::number(result.min));
        ui->lineEdit_fxp_maxRange->setText(QString::number(result.max));
        ui->lineEdit_fxp_precision->setText(QString::number(result.precision));
        ui->lineEdit_fxp_value_dec->setText("");
        ui->lineEdit_fxp_value_hex->setText("");
        ui->lineEdit_fxp_error_abs->setText("");
        ui->lineEdit_fxp_value->setText("");
        ui->lineEdit_fxp_error_rel->setText("");
    }else{
        ui->label_error->setVisible(false);
        if(!result.value_signed) fxp_type_string += "u";
        fxp_type_string += "int_" + QString::number(result.n) + "." + QString::number(result.k);
        ui->label_type->setText(fxp_type_string);
        ui->lineEdit_fxp_minRange->setText(QString::number(result.min));
        ui->lineEdit_fxp_maxRange->setText(QString::number(result.max));
        ui->lineEdit_fxp_precision->setText(QString::number(result.precision));
        ui->lineEdit_fxp_value_dec->setText(QString::number(result.fxp_value_dec));
        ui->lineEdit_fxp_value_hex->setText(QString("0x%1").arg(static_cast<qlonglong>(result.fxp_value_dec),0,16));
        ui->lineEdit_fxp_error_abs->setText(QString::number(result.error_abs));
        ui->lineEdit_fxp_value->setText(QString().setNum(result.fxp_value, 'g', 25));
        ui->lineEdit_fxp_error_rel->setText(QString::number(round(result.error_rel*1000)/1000) + " %");
    }
}
// --------------------------------


