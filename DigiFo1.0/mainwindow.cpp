#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdir.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include<stdio.h>
#include <qinputdialog.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    digifo.start();
    connect(&digifo,SIGNAL(finished_num(int)),this->ui->progressBar,SLOT(setValue(int)));
    connect(&digifo,SIGNAL(finished_num(int)),this,SLOT(action_finished(int)));
    connect(&digifo,SIGNAL(message(QString,int)), this, SLOT(message_add(QString,int)));
    this->ui->progressBar->setMaximum(100);
    ui->dial_randomness->setMaximum(digifo.get_max_randomness());
    ui->dial_randomness->setMinimum(1);
    ui->dial_depth->setMaximum(32);
    ui->dial_depth->setMinimum(1);
    ui->textEdit->setReadOnly(true);
    this->ui->r_md5->setChecked(true);
    init_hash_function();
    QMenu* menu = ui->menuBar->addMenu(tr("&File"));
    actionQuit = menu->addAction("&Quit");
    actionShowMessgae = menu->addAction("Show Message");
    connect(actionQuit,SIGNAL(triggered()), this, SLOT(close()));
    ui->textEdit->setVisible(false);
    actionShowMessgae->setCheckable(true);
    connect(actionShowMessgae, SIGNAL(toggled(bool)), ui->textEdit, SLOT(setVisible(bool)));
    actionGenRandFile = menu->addAction("Generate Random file");
    connect(actionGenRandFile, SIGNAL(triggered()), this, SLOT(generate_random_file()));

    message_add(QString("**********Welcom!***********"),MESSAGE_INFO);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::message_add(QString message, int level)
{
    switch(level)
    {
    case MESSAGE_ERROR:
        message = ">>>ERROR: " + message;
        this->ui->textEdit->setTextColor(QColor("red"));
        break;
    case MESSAGE_WARNNING:
        message = ">>>WARNNING: " + message;
        this->ui->textEdit->setTextColor(QColor("write"));
        break;
    case MESSAGE_INFO:
        message = ">>>INFO: " + message;
        this->ui->textEdit->setTextColor(QColor("green"));
        break;
    case MESSAGE_DEBUG:
        message = ">>>DEBUG: " + message;
        break;
    }
    this->ui->textEdit->append(message);
    this->ui->textEdit->setTextColor(QColor("white"));
}

void MainWindow::init_hash_function()
{
    this->ui->dial_randomness->setValue(1);
    if(ui->r_md5->isChecked())
    {
        if(digifo.get_hash_function() != 0)
            this->digifo.set_hash_function(0);
        return;
    }
    if(ui->r_sha1->isChecked())
    {
        if(digifo.get_hash_function() != 1)
            this->digifo.set_hash_function(1);
        return;
    }
    if(ui->r_sha224->isChecked())
    {
        if(digifo.get_hash_function() != 2)
            this->digifo.set_hash_function(2);
        return;
    }
    if(ui->r_sha256->isChecked())
    {
        if(digifo.get_hash_function() != 3)
            this->digifo.set_hash_function(3);
        return;
    }
    if(ui->r_sha384->isChecked())
    {
        if(digifo.get_hash_function() != 4)
            this->digifo.set_hash_function(4);
        return;
    }
    if(ui->r_sha512->isChecked())
    {
        if(digifo.get_hash_function() != 5)
            this->digifo.set_hash_function(5);
    }
    if(ui->r_sha3_224->isChecked())
    {
        if(digifo.get_hash_function() != 6)
            this->digifo.set_hash_function(6);
        return;
    }
    if(ui->r_sha3_256->isChecked())
    {
        if(digifo.get_hash_function() != 7)
            this->digifo.set_hash_function(7);
        return;
    }
    if(ui->r_sha3_384->isChecked())
    {
        if(digifo.get_hash_function() != 8)
            this->digifo.set_hash_function(8);
        return;
    }
    if(ui->r_sha3_512->isChecked())
    {
        if(digifo.get_hash_function() != 9)
            this->digifo.set_hash_function(9);
        return;
    }
}

void MainWindow::disable_setting(bool set)
{
    this->ui->PB_encode->setDisabled(set);
    this->ui->PB_decode->setDisabled(set);
    this->ui->dial_depth->setDisabled(set);
    this->ui->dial_randomness->setDisabled(set);
    this->ui->PB_open->setDisabled(set);
    this->ui->PB_stateFile->setDisabled(set);
    this->ui->LE_inFile->setDisabled(set);
    this->ui->LE_outFile->setDisabled(set);
    this->ui->LE_passwd->setDisabled(set);
    this->ui->LE_stateFile->setDisabled(set);
    this->ui->r_md5->setDisabled(set);
    this->ui->r_sha1->setDisabled(set);
    this->ui->r_sha224->setDisabled(set);
    this->ui->r_sha256->setDisabled(set);
    this->ui->r_sha384->setDisabled(set);
    this->ui->r_sha512->setDisabled(set);
    this->ui->r_sha3_224->setDisabled(set);
    this->ui->r_sha3_256->setDisabled(set);
    this->ui->r_sha3_384->setDisabled(set);
    this->ui->r_sha3_512->setDisabled(set);
}

void MainWindow::action_finished(int num)
{
    if(num == 100)
    {
        disable_setting(false);
    }
}

void MainWindow::generate_random_file()
{
    bool ok = false;
    unsigned long long size = QInputDialog::getInt(
                this,"File size","File size(KB)",1024,1,5*1024*1024,1,&ok);
    if(!ok) return;
    QString filename = QFileDialog::getSaveFileName(
                this,
                "Save File",
                "./random.out",
                "All files(*)");
    size *= 1024;
   // digifo.gen_random_file(filename, size);
     QMetaObject::invokeMethod(&this->digifo,
                               "gen_random_file",
                               Qt::QueuedConnection,
                               Q_ARG(QString, filename),
                               Q_ARG(unsigned long long, size));
}

void MainWindow::on_PB_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
            this,
            "Open File",
            QDir::currentPath(),
            "All files(*)");

    if (!filename.isNull())
    {
        ui->LE_inFile->setText(filename);
        ui->LE_outFile->setText(filename+".BEo");
    }
}

void MainWindow::on_PB_decode_clicked()
{
    char *text;
    QByteArray ba;


    if(ui->LE_stateFile->text().isEmpty() ||
            ui->LE_passwd->text().isEmpty() ||
            ui->LE_inFile->text().isEmpty() ||
            ui->LE_outFile->text().isEmpty())
    {
        QMessageBox::information(this, "More information requred", "please fill up all blanks", QMessageBox::Ok);
        return;
    }
    digifo.terminate();
    this->ui->progressBar->setValue(0);
    this->digifo.setInFile(ui->LE_inFile->text());
    this->digifo.setOutFile(ui->LE_outFile->text());
    this->digifo.set_rFile(ui->LE_stateFile->text());
    this->digifo.set_randomness(ui->dial_randomness->value());
    this->digifo.set_trans_depth(ui->dial_depth->value());
    ba = ui->LE_passwd->text().toLatin1();
    text = (char *)(ba.data());
    this->digifo.set_password(text, strlen(text));

    disable_setting(true);
    QMetaObject::invokeMethod(&this->digifo, "decode", Qt::QueuedConnection);
}

void MainWindow::on_PB_encode_clicked()
{
    char *text;
    QByteArray ba;


    if(ui->LE_stateFile->text().isEmpty() ||
            ui->LE_passwd->text().isEmpty() ||
            ui->LE_inFile->text().isEmpty() ||
            ui->LE_outFile->text().isEmpty())
    {
        QMessageBox::information(this, "More information required", "please fill up all blanks", QMessageBox::Ok);
        return;

    }
    digifo.terminate();
    this->ui->progressBar->setValue(0);
    this->digifo.setInFile(ui->LE_inFile->text());
    this->digifo.setOutFile(ui->LE_outFile->text());
    this->digifo.set_rFile(ui->LE_stateFile->text());
    this->digifo.set_randomness(ui->dial_randomness->value());
    this->digifo.set_trans_depth(ui->dial_depth->value());
    ba = ui->LE_passwd->text().toLatin1();
    text = (char *)(ba.data());
    this->digifo.set_password(text, strlen(text));

    disable_setting(true);
    QMetaObject::invokeMethod(&this->digifo, "encode", Qt::QueuedConnection);
}

void MainWindow::on_PB_stop_clicked()
{
    digifo.terminate();
    action_finished(100);
}

void MainWindow::on_CB_RFF_toggled(bool checked)
{
    if(checked) {
        QString filename = QFileDialog::getOpenFileName(
                    this,
                    "Open File",
                    QDir::currentPath(),
                    "All files(*)");

        if (!filename.isNull())
        {
            ui->LE_RFF->setText(filename);
            ui->LE_RFF->setEnabled(true);
            digifo.setRFF(ui->LE_RFF->text());

        } else {
            ui->CB_RFF->setChecked(false);
            digifo.setRFF(NULL);
        }

    } else {
        ui->LE_RFF->clear();
        ui->LE_RFF->setEnabled(false);
        digifo.setRFF(NULL);
    }
}

void MainWindow::on_PB_stateFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
            this,
            "Open File",
            QDir::currentPath(),
            "All files(*)");

    if (!filename.isNull())
    {
        ui->LE_stateFile->setText(filename);
    }
}

void MainWindow::on_dial_depth_sliderPressed()
{
    ui->dial_depth->setMaximum(32);
    ui->dial_depth->setMinimum(1);
}

void MainWindow::on_dial_randomness_sliderPressed()
{
    ui->dial_randomness->setMaximum(digifo.get_max_randomness());
    ui->dial_randomness->setMinimum(1);
}


void MainWindow::on_r_md5_toggled(bool checked)
{
    init_hash_function();
}

void MainWindow::on_r_sha1_toggled(bool checked)
{
    init_hash_function();
}

void MainWindow::on_r_sha224_toggled(bool checked)
{
    init_hash_function();
}

void MainWindow::on_r_sha256_toggled(bool checked)
{
    init_hash_function();
}

void MainWindow::on_r_sha384_toggled(bool checked)
{
    init_hash_function();
}

void MainWindow::on_r_sha512_toggled(bool checked)
{
    init_hash_function();
}

void MainWindow::on_r_sha3_224_toggled(bool checked)
{
    init_hash_function();
}

void MainWindow::on_r_sha3_256_toggled(bool checked)
{
    init_hash_function();
}

void MainWindow::on_r_sha3_384_toggled(bool checked)
{
    init_hash_function();
}

void MainWindow::on_r_sha3_512_toggled(bool checked)
{
    init_hash_function();
}
