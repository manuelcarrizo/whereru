#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

#include <QSystemTrayIcon>
#include <QMenu>

#include "findutils.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void showTrayIcon();
    void configureContextMenu();

    void loadScreenPosition();

    void saveScreenPosition();

    void locate(const QString & text);
    void updatedb();
    void update_statistics();

    void resizeEvent(QResizeEvent *);

signals:
    void results_modified();

private slots:
    void on_query_editingFinished();

    void system_tray_clicked(QSystemTrayIcon::ActivationReason reason);
    void toogle_visibility();

    void updatedb_finished();
    void locate_finished(const QStringList & results);
    void statistics_finished(QString message);

private:
    QIcon * tray_icon;
    QSystemTrayIcon * system_tray;
    QMenu * system_tray_menu;

    FindUtils findutils;
};

#endif // MAINWINDOW_H
