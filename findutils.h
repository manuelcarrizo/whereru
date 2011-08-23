#ifndef FINDUTILS_H
#define FINDUTILS_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>

/** @brief Interface to the findutils tools  */
class FindUtils : public QObject
{
    Q_OBJECT

public:
    FindUtils();

    typedef enum {
        MATCH_ALL,
        MATCH_ANY
    } match_t;

    void updatedb();

    void locate(const QString & query);

    void statistics();

    QStringList searchResults();

signals:
    void updatedb_finished();
    void locate_finished(const QStringList & search_results);
    void statistics_finished(QString message);

private slots:
    void updatedb_process_finished();
    void locate_process_finished();
    void statistics_process_finished();

private:
    QProcess * locate_process;
    QProcess * updatedb_process;
    QProcess * statistics_process;

    QString search_key;
    QStringList search_filters;

    QStringList search_results;

    void filter_results(QList<QByteArray> & matches);

    void create_db_path();
};

#endif // FINDUTILS_H
