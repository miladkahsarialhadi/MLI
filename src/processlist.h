#ifndef PROCESSLIST_H
#define PROCESSLIST_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtCore>
#include <QtGui>

#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <TlHelp32.h>
#include <tchar.h>
#include <psapi.h>

namespace Ui {
class ProcessList;
}

class ProcessList : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProcessList(QWidget *parent = nullptr);
    ~ProcessList();

private slots:
    void on_pushButtonClose_clicked();

    void on_pushButtonRefreshList_clicked();

private:
    Ui::ProcessList *ui;
};

#endif // PROCESSLIST_H
