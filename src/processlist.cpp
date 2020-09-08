#include "processlist.h"
#include "ui_processlist.h"



ProcessList::ProcessList(QWidget *parent) : QMainWindow(parent), ui(new Ui::ProcessList)
{
    ui->setupUi(this);

    DWORD processes[1024];
    DWORD needed;
    DWORD process_list;

    if (!EnumProcesses(processes, sizeof(processes), &needed))
    {
        QMessageBox::warning(this, "Failed OPS", "EnumProcesses failed...");
        return;
    }

    process_list = needed / sizeof(DWORD);

    for (uint32_t i = 0; i < process_list; i++)
    {
        if (processes[i] != 0)
        {
            char process_name[MAX_PATH] = "<UKNOWN PROCESS>";
            HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);

            if (process != NULL)
            {
                HMODULE handle_module;
                needed = 0;

                if (EnumProcessModules(process, &handle_module, sizeof(handle_module), &needed))
                {
                    GetModuleBaseNameA(process, handle_module, process_name, sizeof(process_name) / sizeof(TCHAR));

                    QString process_name_qstr = (LPSTR)process_name;
                    auto first = new QTableWidgetItem(QString(process_name_qstr));
                    auto second = new QTableWidgetItem(QString::number(processes[i]));

                    first->setForeground(QBrush("red"));
                    second->setForeground(QBrush("red"));

                    first->setBackground(QBrush("yellow"));
                    second->setBackground(QBrush("yellow"));

                    const auto row = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(row);
                    ui->tableWidget->setItem(row, 0, first);
                    ui->tableWidget->setItem(row, 1, second);
                }

            }
        }
    }
}

ProcessList::~ProcessList()
{
    delete ui;
}

void ProcessList::on_pushButtonClose_clicked()
{
    close();
}

void ProcessList::on_pushButtonRefreshList_clicked()
{
    ui->tableWidget->setRowCount(0);
    DWORD processes[1024];
    DWORD needed;
    DWORD process_list;

    if (!EnumProcesses(processes, sizeof(processes), &needed))
    {
        QMessageBox::warning(this, "Failed OPS", "EnumProcesses failed...");
        return;
    }

    process_list = needed / sizeof(DWORD);

    for (uint32_t i = 0; i < process_list; i++)
    {
        if (processes[i] != 0)
        {
            char process_name[MAX_PATH] = "<UKNOWN PROCESS>";
            HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);

            if (process != NULL)
            {
                HMODULE handle_module;
                needed = 0;

                if (EnumProcessModules(process, &handle_module, sizeof(handle_module), &needed))
                {
                    GetModuleBaseNameA(process, handle_module, process_name, sizeof(process_name) / sizeof(TCHAR));

                    QString process_name_qstr = (LPSTR)process_name;
                    auto first = new QTableWidgetItem(QString(process_name_qstr));
                    auto second = new QTableWidgetItem(QString::number(processes[i]));

                    first->setForeground(QBrush("red"));
                    second->setForeground(QBrush("red"));

                    first->setBackground(QBrush("yellow"));
                    second->setBackground(QBrush("yellow"));

                    const auto row = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(row);
                    ui->tableWidget->setItem(row, 0, first);
                    ui->tableWidget->setItem(row, 1, second);
                }

            }
        }
    }
}
