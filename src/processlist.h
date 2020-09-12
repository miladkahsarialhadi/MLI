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

signals:
    void m_new_pid(const uint64_t arg_pid);

private slots:
    void on_pushButtonClose_clicked();

    void on_pushButtonRefreshList_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::ProcessList *ui;
};

#endif // PROCESSLIST_H
