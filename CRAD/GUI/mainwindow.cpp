#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize QTimer
    timer = new QTimer(this);

    // Start QTimer with interval of 200 milliseconds (0.2 seconds)
    timer->start(200);

    // Connect the fetch button's clicked signal to the on_fetchMetricsButton_clicked slot
    connect(ui->fetchMetricsButton, &QPushButton::clicked, this, &MainWindow::on_fetchMetricsButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::on_fetchMetricsButton_clicked()
{
    // Connect QTimer's timeout signal to updateMetrics slot
    connect(timer, &QTimer::timeout, this, &MainWindow::updateMetrics);

    // Call updateMetrics when the fetch button is clicked
    updateMetrics();
}

void MainWindow::updateMetrics()
{
    QString containerName = ui->containerNameLineEdit->text();
    QString command = "docker stats --no-stream " + containerName;

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        ui->cpuLabel->setText("Error: Unable to create pipe.");
        ui->memoryLabel->setText("");
        return;
    }

    // Initialize STARTUPINFO structure
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;

    // Initialize PROCESS_INFORMATION structure
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    // Create the process
    BOOL success = CreateProcessA(NULL,   // Module name (NULL means use command line)
                                  command.toLocal8Bit().data(), // Command line
                                  NULL,   // Process security attributes
                                  NULL,   // Thread security attributes
                                  TRUE,   // Inherit handles from parent process
                                  CREATE_NO_WINDOW, // Creation flags (prevent console window)
                                  NULL,   // Environment block (use parent's)
                                  NULL,   // Current directory (use parent's)
                                  &si,    // STARTUPINFO
                                  &pi);   // PROCESS_INFORMATION

    CloseHandle(hWritePipe);

    if (!success) {
        ui->cpuLabel->setText("Error: Unable to create process.");
        ui->memoryLabel->setText("");
        CloseHandle(hReadPipe);
        return;
    }

    CloseHandle(hWritePipe);

    DWORD dwRead;
    char buffer[4096];
    QString result;
    while (ReadFile(hReadPipe, buffer, sizeof(buffer), &dwRead, NULL)) {
        if (dwRead == 0)
            break;
        buffer[dwRead] = '\0';
        result += QString::fromLocal8Bit(buffer);
    }

    CloseHandle(hReadPipe);

    // Parse result to extract CPU and memory metrics
    QStringList lines = result.split('\n');
    if (lines.size() < 2) {
        ui->cpuLabel->setText("Error: No metrics available.");
        ui->memoryLabel->setText("");
        return;
    }

    QString cpuMetric, memoryMetric;
    QStringList metrics = lines[1].simplified().split(' ');
    for (int i = 0; i < metrics.size(); ++i) {
        if (metrics[i].endsWith('%')) {
            if (cpuMetric.isEmpty())
                cpuMetric = metrics[i]; // Update CPU metric
            else
                memoryMetric = metrics[i]; // Update memory metric
        } else if (metrics[i].endsWith("MiB")) {
            memoryMetric = metrics[i]; // Update memory metric
        }
    }

    if (cpuMetric.isEmpty() || memoryMetric.isEmpty()) {
        ui->cpuLabel->setText("Error: No metrics available.");
        ui->memoryLabel->setText("");
        return;
    }

    ui->cpuLabel->setText("CPU Usage: " + cpuMetric);
    ui->memoryLabel->setText("Memory Usage: " + memoryMetric);

    // Check for buffer overflow
    double cpuPercentage = cpuMetric.left(cpuMetric.length() - 1).toDouble();
    double memoryPercentage = memoryMetric.left(memoryMetric.length() - 3).toDouble();
    if (cpuPercentage > 20.0 && memoryPercentage > 50.0) {
        ui->alertLabel->setText("BUFFER OVERFLOW DETECTED!");
    } else {
        ui->alertLabel->setText("");
    }
}
