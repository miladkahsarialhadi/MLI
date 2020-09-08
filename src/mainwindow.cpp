#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget::setFixedSize(650, 330);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonLibrary_clicked()
{
    m_library_path = QFileDialog::getOpenFileName();

    if(m_library_path.isEmpty())
        QMessageBox::warning(this, "Select Library", "Please select a dll.");
    else
    {
        ui->lineEditLibrary->setText(m_library_path);
        g_dllPath = m_library_path.toStdString();
    }
}

void MainWindow::on_pushButtonButtonInject_clicked()
{
    // Reset working environment to empty strings and representive addresses
    m_converter_dll.str(std::string());
    m_converter_api.str(std::string());
    m_converter_thread.str(std::string());
    ui->labelDllAllocationAddress->setText("");
    ui->labelLoadLibraryAddress->setText("");
    ui->labelRemoteThreadAddress->setText("");

    // Get process pid from user
    m_process_id = ui->lineEditProcessID->text();

    // 1. Get process handle
    uint16_t target_pid = (uint16_t)std::stoi(m_process_id.toStdString());

    if(!target_pid)
    {
        QMessageBox::warning(this, "Failed Ops", "Getting remote target process ID failed...");
        return;
    }

    m_handle_remote_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, target_pid);

    if (m_handle_remote_process == NULL)
    {
        QMessageBox::warning(this, "Failed OPS", "Acquiring a handle to the remote target process failed...");
        return;
    }

    // 2. copy file path in szLibPath
    GetFullPathNameA(g_dllPath.c_str(), _MAX_PATH, m_library_full_path_name, NULL);
    strcpy_s(m_library_path_copied, m_library_full_path_name);

    // 3. Allocate memory in the remote process for szLibPath
    m_mapped_library_address = VirtualAllocEx(m_handle_remote_process, NULL, sizeof(m_library_path_copied), MEM_COMMIT, PAGE_READWRITE);

    if (m_mapped_library_address == NULL) {
        QMessageBox::warning(this, "Failed OPS", "Allocating space for our DLL path in the remote target process's virtual memory space failed...");
        CloseHandle(m_handle_remote_process);
        return;
    }

    m_address = static_cast<LPVOID>(m_mapped_library_address);
    m_converter_dll << m_address;
    ui->labelDllAllocationAddress->setText("0x" + QString::fromStdString(m_converter_dll.str()));


    // 4. Write szLibPath to the allocated memory
    bool write_status = WriteProcessMemory(m_handle_remote_process,
                                           m_mapped_library_address,
                                           (void*)m_library_path_copied,
                                           sizeof(m_library_path_copied),
                                           NULL);

    if(!write_status)
    {
        statusBar()->showMessage("WriteProcessMemory was unsuccessful!");
        QMessageBox::warning(this, "Failed OPS", "GetLastError() for failed WriteProcessMemory() call: " + GetLastError());
        CloseHandle(m_handle_remote_process);
        return;
    }
    else
    {
        statusBar()->showMessage("WriteProcessMemory was successful!", 2);
    }

    // 5. Force remote process to load dll
    m_handle_kernel32 = GetModuleHandleA("Kernel32");

    m_address = static_cast<LPVOID>(GetProcAddress(m_handle_kernel32, "LoadLibraryA"));
    m_converter_api << m_address;
    ui->labelLoadLibraryAddress->setText("0x" + QString::fromStdString(m_converter_api.str()));

    m_handle_remote_thread = CreateRemoteThread(m_handle_remote_process,
                                                NULL,
                                                0,
                                                (LPTHREAD_START_ROUTINE)GetProcAddress(m_handle_kernel32, "LoadLibraryA"),
                                                m_mapped_library_address,
                                                0,
                                                NULL);

    if (m_handle_remote_thread == NULL)
    {
        QMessageBox::warning(this, "Failed OPS", "CreateRemoteThread failed - ERROR Code:" + GetLastError());
        return;
    }

    m_address = static_cast<LPVOID>(m_handle_remote_thread);
    m_converter_thread << m_address;
    ui->labelRemoteThreadAddress->setText("0x" + QString::fromStdString(m_converter_thread.str()));

    // Free our handle on the remote thread
    CloseHandle(m_handle_remote_thread);

    // Free our handle on the remote process
    CloseHandle(m_handle_remote_process);
}

void MainWindow::on_pushButtonButtonClose_clicked()
{
    close();
}

void MainWindow::on_pushButtonProcessIdList_clicked()
{
    m_process_list_window.show();
}

void MainWindow::on_actionAboutMLI_triggered()
{
    m_about_window.show();
}

void MainWindow::on_actionProcessList_triggered()
{
    m_process_list_window.show();
}

void MainWindow::on_actionInjection_triggered()
{
    // Reset working environment to empty strings and representive addresses
    m_converter_dll.str(std::string());
    m_converter_api.str(std::string());
    m_converter_thread.str(std::string());
    ui->labelDllAllocationAddress->setText("");
    ui->labelLoadLibraryAddress->setText("");
    ui->labelRemoteThreadAddress->setText("");

    // Get process pid from user
    m_process_id = ui->lineEditProcessID->text();

    // 1. Get process handle
    uint16_t target_pid = (uint16_t)std::stoi(m_process_id.toStdString());

    if(!target_pid)
    {
        QMessageBox::warning(this, "Failed Ops", "Getting remote target process ID failed...");
        return;
    }

    m_handle_remote_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, target_pid);

    if (m_handle_remote_process == NULL)
    {
        QMessageBox::warning(this, "Failed OPS", "Acquiring a handle to the remote target process failed...");
        return;
    }

    // 2. copy file path in szLibPath
    GetFullPathNameA(g_dllPath.c_str(), _MAX_PATH, m_library_full_path_name, NULL);
    strcpy_s(m_library_path_copied, m_library_full_path_name);

    // 3. Allocate memory in the remote process for szLibPath
    m_mapped_library_address = VirtualAllocEx(m_handle_remote_process, NULL, sizeof(m_library_path_copied), MEM_COMMIT, PAGE_READWRITE);

    if (m_mapped_library_address == NULL) {
        QMessageBox::warning(this, "Failed OPS", "Allocating space for our DLL path in the remote target process's virtual memory space failed...");
        CloseHandle(m_handle_remote_process);
        return;
    }

    m_address = static_cast<LPVOID>(m_mapped_library_address);
    m_converter_dll << m_address;
    ui->labelDllAllocationAddress->setText("0x" + QString::fromStdString(m_converter_dll.str()));


    // 4. Write szLibPath to the allocated memory
    bool write_status = WriteProcessMemory(m_handle_remote_process,
                                           m_mapped_library_address,
                                           (void*)m_library_path_copied,
                                           sizeof(m_library_path_copied),
                                           NULL);

    if(!write_status)
    {
        statusBar()->showMessage("WriteProcessMemory was unsuccessful!");
        QMessageBox::warning(this, "Failed OPS", "GetLastError() for failed WriteProcessMemory() call: " + GetLastError());
        CloseHandle(m_handle_remote_process);
        return;
    }
    else
    {
        statusBar()->showMessage("WriteProcessMemory was successful!", 2);
    }

    // 5. Force remote process to load dll
    m_handle_kernel32 = GetModuleHandleA("Kernel32");

    m_address = static_cast<LPVOID>(GetProcAddress(m_handle_kernel32, "LoadLibraryA"));
    m_converter_api << m_address;
    ui->labelLoadLibraryAddress->setText("0x" + QString::fromStdString(m_converter_api.str()));

    m_handle_remote_thread = CreateRemoteThread(m_handle_remote_process,
                                                NULL,
                                                0,
                                                (LPTHREAD_START_ROUTINE)GetProcAddress(m_handle_kernel32, "LoadLibraryA"),
                                                m_mapped_library_address,
                                                0,
                                                NULL);

    if (m_handle_remote_thread == NULL)
    {
        QMessageBox::warning(this, "Failed OPS", "CreateRemoteThread failed - ERROR Code:" + GetLastError());
        return;
    }

    m_address = static_cast<LPVOID>(m_handle_remote_thread);
    m_converter_thread << m_address;
    ui->labelRemoteThreadAddress->setText("0x" + QString::fromStdString(m_converter_thread.str()));

    // Free our handle on the remote thread
    CloseHandle(m_handle_remote_thread);

    // Free our handle on the remote process
    CloseHandle(m_handle_remote_process);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
