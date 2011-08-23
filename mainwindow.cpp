#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "stdio.h"
#include "time.h"

#include <QDir>
#include <QSettings>
#include <QRect>
#include <QFont>

#include "settings.h"
#include "defines.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(APP_FRIENDLY_NAME);

    loadScreenPosition();

    showTrayIcon();
    configureContextMenu();

    connect(&findutils, SIGNAL(updatedb_finished()), this, SLOT(updatedb_finished()));
    connect(&findutils, SIGNAL(locate_finished(const QStringList &)), this, SLOT(locate_finished(const QStringList &)));
    connect(&findutils, SIGNAL(statistics_finished(QString)), this, SLOT(statistics_finished(QString)));

    updatedb();
}

MainWindow::~MainWindow()
{
    saveScreenPosition();
    saveGeometry();

    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    int extraWidth = width() - minimumWidth();
    int extraHeight = height() - minimumHeight();

    ui->query->setGeometry(10, 10, 620 + extraWidth, 24);
    ui->results->setGeometry(10, 40, 620 + extraWidth, 410 + extraHeight);
}

void MainWindow::showTrayIcon()
{
    QIcon * tray_icon = new QIcon();
    tray_icon->addFile(QString::fromUtf8(":images/images/icon.png"), QSize(), QIcon::Normal, QIcon::Off);

    system_tray_menu = new QMenu(this);

    QAction * visibility = new QAction("Show/Hide", this);
    connect(visibility, SIGNAL(triggered()), this, SLOT(toogle_visibility()));
    system_tray_menu->addAction(visibility);


    QAction * copy = new QAction("Copy Selection", this);
    copy->setShortcut(QKeySequence::Copy);
    connect(copy, SIGNAL(triggered()), ui->results, SLOT(copySelection()));
    this->addAction(copy);
    system_tray_menu->addAction(copy);


    system_tray = new QSystemTrayIcon(this);
    system_tray->setContextMenu(system_tray_menu);
    system_tray->setIcon(*tray_icon);
    connect(system_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(system_tray_clicked(QSystemTrayIcon::ActivationReason)));

    system_tray->show();
}

void MainWindow::configureContextMenu()
{

}

void MainWindow::system_tray_clicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger) {
        toogle_visibility();
    }
}

void MainWindow::toogle_visibility()
{
    if(isVisible())
        hide();
    else
        showNormal();
}


void MainWindow::loadScreenPosition()
{
    QRect rect = Settings::geometry();
    move(rect.topLeft());
    resize(rect.size());
}

void MainWindow::saveScreenPosition()
{
    Settings::geometry(this->geometry());
}

void MainWindow::on_query_editingFinished()
{
    ui->query->setEnabled(false);

    QString query = ui->query->text().trimmed();

    if(query.size()) {
        ui->results->clearContents();
        locate(query);
    }
    ui->query->setEnabled(true);
}

void MainWindow::locate(const QString & key)
{
    findutils.locate(key);
}

void MainWindow::updatedb()
{
    ui->query->setEnabled(false);

    findutils.updatedb();
}

void MainWindow::updatedb_finished()
{
    ui->query->setEnabled(true);
    ui->query->setFocus();

    findutils.statistics();
}

void MainWindow::statistics_finished(QString message)
{
    ui->statusBar->showMessage(message);
}

void MainWindow::locate_finished(const QStringList & results)
{
    ui->query->setEnabled(false);

    ui->results->fillList(results);

    ui->query->setEnabled(true);
}

// use Item->tableWidget()->isRowHidden( item->tableWidget()->row( item ) )
// to get the size of the visible items
