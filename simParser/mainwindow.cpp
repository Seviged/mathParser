#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "mathparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //parser *prs = new parser;

    //QObject::connect(prs, SIGNAL(sendDebug(QString)), this, SLOT(debugOut(QString)));
    //QObject::connect(prs, SIGNAL(sendResult(QString)), this, SLOT(resultOut(QString)));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutSlot()));
}

void MainWindow::aboutSlot()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About me");
    msgBox.setText("This parser is very simple to use. \nJust use + - * / x ^ : \\");
    msgBox.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendString()
{
    MathParser *parser = new MathParser;
    QStringList res;
    res = parser->parse(ui->lineEdit->text());

    if (res.at(1).toInt() == -1) {
        debugOut(res.at(0));
    }
    else
    {
        resultOut(res.at(0));
        debugOut(res.at(2));
    }
}

void MainWindow::debugOut(QString str)
{
    ui->plainTextEdit->appendPlainText(str);
}

void MainWindow::resultOut(QString str)
{
    ui->result->setText(str);
}
