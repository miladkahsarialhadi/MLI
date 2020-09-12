#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include "processlist.h"
#include "about.h"

#include "globals.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void ProcessIdShow(const uint64_t arg_pid);

private slots:
    void on_pushButtonLibrary_clicked();

    void on_pushButtonButtonInject_clicked();

    void on_pushButtonButtonClose_clicked();

    void on_pushButtonProcessIdList_clicked();

    void on_actionAboutMLI_triggered();

    void on_actionProcessList_triggered();

    void on_actionInjection_triggered();

    void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;
    ProcessList m_process_list_window;
    About m_about_window;

    QString m_library_path;
    QString m_process_id = 0;

    HANDLE m_handle_remote_thread;
    HANDLE m_handle_remote_process;
    void*  m_mapped_library_address = 0;  // the address (in the remote process) where szLibPath will be copied to;

    HMODULE m_handle_kernel32;
    char m_library_full_path_name[_MAX_PATH];
    char m_library_path_copied[_MAX_PATH];

    LPVOID m_address;
    std::stringstream m_converter_dll;
    std::stringstream m_converter_api;
    std::stringstream m_converter_thread;
};
#endif // MAINWINDOW_H
