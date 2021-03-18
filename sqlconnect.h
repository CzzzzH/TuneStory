#ifndef SQLCONNECT_H
#define SQLCONNECT_H

#include <QObject>
#include <QtSql>
#include <QtConcurrent/QtConcurrent>

class UI;

class SqlConnect : public QObject // A singleton class for SQL connecting
{
    Q_OBJECT

private:

    explicit SqlConnect(QObject *parent = nullptr);
    SqlConnect(const SqlConnect &) Q_DECL_EQ_DELETE;
    SqlConnect & operator= (SqlConnect _RightHandObj) Q_DECL_EQ_DELETE;

public:

    static UI *_UI;
    static QSqlDatabase _Database;
    static SqlConnect * instance();
    static bool UploadScore();
    static bool ReadHighScore();
    static bool DownloadScore();
    static bool CreateAccount();
    static bool Login();
    static void Initialize();
    ~SqlConnect();

};

#endif // SQLCONNECT_H
