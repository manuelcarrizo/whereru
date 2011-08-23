#include "filelistwidget.h"

#include <math.h>
#include <sstream>

#include <QApplication>
#include <QClipboard>

#include <QAction>
#include <QFileIconProvider>

#include <QUrl>
#include <QDesktopServices>

FileListWidget::FileListWidget(QWidget *parent)
    : QTableWidget(parent)
{
    createContextMenu();
    connectSignals();
}

void FileListWidget::createContextMenu()
{
    QAction * open_file = new QAction("Open file", this);
    this->addAction(open_file);
    connect(open_file, SIGNAL(triggered()), this, SLOT(menuOpenFile()));

    QAction * open_path = new QAction("Open path", this);
    this->addAction(open_path);
    connect(open_path, SIGNAL(triggered()), this, SLOT(menuOpenPath()));

    QAction * separator = new QAction(NULL);
    separator->setSeparator(true);
    this->addAction(separator);

    QAction * copy_name = new QAction("Copy file name", this);
    this->addAction(copy_name);
    connect(copy_name, SIGNAL(triggered()), this, SLOT(menuCopyName()));

    QAction * copy_path = new QAction("Copy path name", this);
    this->addAction(copy_path);
    connect(copy_path, SIGNAL(triggered()), this, SLOT(menuCopyPath()));

    QAction * copy_full = new QAction("Copy full path", this);
    this->addAction(copy_full);
    connect(copy_full, SIGNAL(triggered()), this, SLOT(menuCopyFull()));

    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void FileListWidget::menuOpenFile()
{
    this->openFileAt(selected_row, selected_column);
}

void FileListWidget::menuOpenPath()
{
    this->openPathAt(selected_row, selected_column);
}

void FileListWidget::menuCopyName()
{
    QString name = this->item(selected_row, COLUMN_NAME)->text();
    QApplication::clipboard()->setText(name);
}

void FileListWidget::menuCopyPath()
{
    QString path = this->item(selected_row, COLUMN_PATH)->text();
    QApplication::clipboard()->setText(path);
}

void FileListWidget::menuCopyFull()
{
    QString name = this->item(selected_row, COLUMN_NAME)->text();
    QString path = this->item(selected_row, COLUMN_PATH)->text();
    QApplication::clipboard()->setText(path + "/" + name);
}

void FileListWidget::connectSignals()
{
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(onCellDoubleClicked(int, int)));
    connect(this, SIGNAL(cellPressed(int,int)), this, SLOT(onCellPressed(int, int)));
}

void FileListWidget::onCellDoubleClicked(int row, int column)
{
    printf("FileListWidget::onCellDoubleClicked(%d, %d)\n", row, column);

    if(column == COLUMN_NAME) {
        openFileAt(row, column);
    }
    else if(column == COLUMN_PATH) {
        openPathAt(row, column);
    }
}

void FileListWidget::onCellPressed(int row, int column)
{
    selected_row = row;
    selected_column = column;
}

void FileListWidget::openFileAt(int row, int )
{
    QString name = this->item(row, COLUMN_NAME)->text();
    QString path = this->item(row, COLUMN_PATH)->text();
    QDesktopServices::openUrl(QUrl("file:///" + path + "/" + name, QUrl::TolerantMode));
}

void FileListWidget::openPathAt(int row, int )
{
    QString path = this->item(row, COLUMN_PATH)->text();
    QDesktopServices::openUrl(QUrl("file:///" + path, QUrl::TolerantMode));
}

void FileListWidget::copySelection()
{
    std::stringstream ss;
    QList<QTableWidgetSelectionRange> selection = this->selectedRanges();
    for(int i = 0; i < selection.size(); i++) {
        int top_row = selection.at(i).topRow();
        int bottom_row = selection.at(i).bottomRow();

        int left_column = selection.at(i).leftColumn();
        int right_column = selection.at(i).rightColumn();

        for(int row = top_row; row <= bottom_row; row++ ) {
            for(int column = right_column; column >= left_column; column--) {
                ss << this->item(row, column)->text().toStdString();

                if(column == 1) {
                    ss << "/";
                }
            }

            ss << "\n";
        }
    }

    QApplication::clipboard()->setText(ss.str().c_str());
}

void FileListWidget::fillList(const QStringList & results)
{
    QFileIconProvider icon_provider;
    int matches_count = 0;
    int result_count = results.size();
    for(int index = 0; index < result_count; index++) {
        QString line = results.at(index);
        QFileInfo info(line);

        QTableWidgetItem * nameItem = new QTableWidgetItem(info.fileName());
        nameItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        nameItem->setIcon(icon_provider.icon(info));
        this->setItem(matches_count, COLUMN_NAME, nameItem);

        QTableWidgetItem * pathItem = new QTableWidgetItem(info.absolutePath());
        pathItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        this->setItem(matches_count, COLUMN_PATH, pathItem);

        if(info.isFile()) {
            int file_size = ceil( (info.size() / 1024) + 0.5);
            QTableWidgetItem * sizeItem = new QTableWidgetItem(QString("%1 Kb").arg(file_size));
            sizeItem->setFlags(Qt::ItemIsEnabled);
            this->setItem(matches_count, COLUMN_SIZE, sizeItem);
        }

        matches_count++;

    }

    this->setRowCount(matches_count);
}
