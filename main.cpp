#include <QApplication>
#include "add_comment.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(comment);
    QApplication a(argc, argv);
    a.setApplicationName("Add comments");

    MainWindow w;
    w.show();

    return a.exec();
}
