#ifndef NEEDEDTYPES_H
#define NEEDEDTYPES_H

#define Org "GiVMiaKiS"
#define DBName "IBC.db"
#define AppVersion "0.1"
#define AppName "I&B Client"
#ifdef Q_WS_WIN
#   define AppFolder "/AppData/Local/GiVMiaKiS/IBC/"
#else
#   define AppFolder "/.GiVMiaKiS/IBC/"
#endif


#include <QDir>
#include <QFile>
#include <QMenu>
#include <QFrame>
#include <QLabel>
#include <QAction>
#include <QSpinBox>
#include <QToolBar>
#include <QMenuBar>
#include <QSplitter>
#include <QSqlQuery>
#include <QTextEdit>
#include <QSettings>
#include <QGroupBox>
#include <QSqlError>
#include <QLineEdit>
#include <QListView>
#include <QSqlField>
#include <QSqlRecord>
#include <QTableView>
#include <QPushButton>
#include <QListWidget>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QKeySequence>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>

#include <QDebug>
#include <QtGlobal>

enum Person {
    Id          = 0,
    FirstName   = 1,
    MidleName   = 2,
    LastName    = 3,
    DOB         = 4,
    Country     = 5,
    City        = 6,
    Address     = 7,
    Mobile      = 8,
    Passport    = 9
};

enum Info {
    id          = 0,
    Referer     = 1,
    AmountSpent = 2,
    PresentMoney= 3,
    Referals    = 4
};

QString cq1( int );
QString cq2( int );
QString iptusn( Info ); // Info properties to user shown names
QString pptusn( Person ); // Person properties to user shown names


#endif // NEEDEDTYPES_H
