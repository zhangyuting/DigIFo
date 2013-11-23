#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

/* time %1 of 301.jpg
 * 1. Default: time 1173ms
 * 2. No thread_random:  time 974ms
 * 3. No hash:   time 594ms
 * 4. No write file: time 1012ms
 *
 * thread_random:  199ms
 * hash:  579ms
 * write file: 161ms
 */
