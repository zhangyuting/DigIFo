#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DigiFo.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        DigiFo digifo;
        QAction* actionQuit;
        QAction* actionShowMessgae;
        QAction* actionGenRandFile;
        void disable_setting(bool set);
        void init_hash_function();

signals:

        public slots:
        void message_add(QString message, int level);

        private slots:
        void generate_random_file();
        void on_PB_open_clicked();
        void on_PB_decode_clicked();
        void on_PB_encode_clicked(); 
        void action_finished(int num);
        void on_PB_stop_clicked();
        void on_CB_RFF_toggled(bool checked);
        void on_PB_stateFile_clicked();
        void on_dial_depth_sliderPressed();
        void on_dial_randomness_sliderPressed();
        void on_r_md5_toggled(bool checked);
        void on_r_sha1_toggled(bool checked);
        void on_r_sha224_toggled(bool checked);
        void on_r_sha256_toggled(bool checked);
        void on_r_sha384_toggled(bool checked);
        void on_r_sha512_toggled(bool checked);
        void on_r_sha3_224_toggled(bool checked);
        void on_r_sha3_256_toggled(bool checked);
        void on_r_sha3_384_toggled(bool checked);
        void on_r_sha3_512_toggled(bool checked);
};

#endif // MAINWINDOW_H
