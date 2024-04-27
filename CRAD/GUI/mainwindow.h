// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_fetchMetricsButton_clicked();
    void updateMetrics();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QString exec(const char* cmd);
};

#endif // MAINWINDOW_H
