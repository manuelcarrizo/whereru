#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QtGui/QTableWidget>
#include <QStringList>

class FileListWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit FileListWidget(QWidget *parent = 0);

    typedef enum {
        COLUMN_NAME = 0,
        COLUMN_PATH,
        COLUMN_SIZE
    } COLUMN_TYPE;

    void fillList(const QStringList & results);

public slots:
    void copySelection();

private:
    int selected_row;
    int selected_column;

    void createContextMenu();

    void connectSignals();

    void openFileAt(int row, int column);

    void openPathAt(int row, int column);

private slots:
    void onCellDoubleClicked(int row, int column);

    void onCellPressed(int row, int column);


    void menuOpenFile();

    void menuOpenPath();

    void menuCopyName();

    void menuCopyPath();

    void menuCopyFull();
};

#endif // FILELISTWIDGET_H
