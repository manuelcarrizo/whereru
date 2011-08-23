#include "findutils.h"

#include "defines.h"

#include <QDir>

static const char * LOCATE_BIN = "locate";
static const char * UPDATEDB_BIN = "updatedb";

static QString APP_HOME = QDir::homePath() + "/." + APP_NAME;
static QString DB_PATH = APP_HOME + "/mlocate.db";

static const char * IGNORED_FOLDERS = "\".git .bzr .hg .svn\"";

FindUtils::FindUtils()
{
    // Connect the process signals
    locate_process = new QProcess(this);
    connect(locate_process, SIGNAL(finished(int)),
            this, SLOT(locate_process_finished()));

    updatedb_process = new QProcess(this);
    connect(updatedb_process, SIGNAL(finished(int)),
            this, SLOT(updatedb_process_finished()));

    statistics_process = new QProcess(this);
    connect(statistics_process, SIGNAL(finished(int)),
            this, SLOT(statistics_process_finished()));

    create_db_path();
}

void FindUtils::updatedb()
{
    QStringList arguments;
    arguments << "--require-visibility" << "0"
              << "--add-prunenames" << IGNORED_FOLDERS
              << "--output" << DB_PATH;

    updatedb_process->start(UPDATEDB_BIN, arguments);
}

void FindUtils::create_db_path()
{
    QDir helper;
    helper.mkpath(APP_HOME);
}

void FindUtils::locate(const QString & q)
{
    QString query = q.trimmed();
    if(query.isEmpty())
        return;

    QStringList filters = query.split(" ");

    QString key = filters.at(0);
    filters.removeAt(0);

    search_filters = filters;

    /*
    if(search_key.compare(key) == 0) {
        // the search key hasn't changed, there is no need
        // to call locate again, we only need to filter the results

        puts("trying to search for the same key again");
        locate_process_finished();
        return;
    }
    */

    search_results.clear();

    search_key = key;

    QStringList arguments;
    arguments << "-d" << DB_PATH << key;

    locate_process->start(LOCATE_BIN, arguments);
    locate_process->waitForFinished();
}

void FindUtils::statistics()
{
    QStringList arguments;
    arguments << "-S" << "-d" << DB_PATH;

    statistics_process->start(LOCATE_BIN, arguments);
}

void FindUtils::updatedb_process_finished()
{
    emit updatedb_finished();
}

void FindUtils::locate_process_finished()
{
    QByteArray output = locate_process->readAllStandardOutput().trimmed();
    QList<QByteArray> matches = output.split('\n');

    filter_results(matches);

    emit locate_finished(search_results);
}

void FindUtils::filter_results(QList<QByteArray> & matches)
{
    for(int r = 0; r < matches.size(); r++) {
        // for each result returned by locate
        QString line = matches.at(r);
        if(line.size() == 0)
            continue;

        bool matches = true;
        for(int f = 0; f < search_filters.size(); f++) {
            // check that contains all the filters
            QString filter = search_filters.at(f);

            if(!line.contains(filter, Qt::CaseInsensitive)) {
                matches = false;
                break;
            }
        }

        // Ignore the ones that don't have all the filter words
        if(matches) {
            search_results << line;
        }
    }
}

void FindUtils::statistics_process_finished()
{
    QByteArray output = statistics_process->readAllStandardOutput().trimmed();
    QList<QByteArray> rows = output.split('\n');

    /* output of 'locate -S -d {DB}' is
       Database {DB}:
        M directories
        N files
        XXX bytes in file names
        YYY bytes used to store database
    */
    if(rows.size() > 3) {
        QString directories_line = rows.at(1).data();
        QString files_line = rows.at(2).data();

        emit statistics_finished(
            QString("Searching %1 - %2").arg(directories_line).arg(files_line)
        );
    }
}
