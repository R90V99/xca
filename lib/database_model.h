/* vi: set sw=4 ts=4:
 *
 * Copyright (C) 2020 Christian Hohnstaedt.
 *
 * All rights reserved.
 */

#ifndef __DATABASE_MODEL_H
#define __DATABASE_MODEL_H

#include <QList>
#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "sql.h"
#include "db_base.h"

typedef QMap<QString, QString> DbMap;

class database_model: public QObject
{
	Q_OBJECT

	private:
		QList<db_base*> models;
		int dbTimer;
		void openSqlDB();
		QSqlError initSqlDB();
		QString dbName;
		bool checkForOldDbFormat(const QString &dbfile) const;
		int verifyOldDbPass(const QString &dbname) const;
		void importOldDatabase(const QString &dbfile);
		QString get_default_db() const;
		QString checkPre2Xdatabase() const;
		int initPass(const QString &dbName,
				const QString &passhash) const;

	public:
		database_model(const QString &dbName,
				const QString &pass = QString());
		~database_model();
		void restart_timer();
		const QString &dbname() const
		{
			return dbName;
		}
		template <class T> T *model() const
		{
			foreach(db_base *model, models) {
				T *m = dynamic_cast<T*>(model);
				if (m)
					return m;
			}
			return NULL;
		}
		void timerEvent(QTimerEvent *event);
		void dump_database(const QString &dirname) const;
		QList<db_base*> getModels() const
		{
			return models;
		}

		static void as_default_database(const QString &db);
		static DbMap splitRemoteDbName(const QString &db);
		static bool isRemoteDB(const QString &db);
		static void openDatabase(const QString &descriptor,
					 const QString &pass);
		static void openRemoteDatabase(const QString &connName,
						const DbMap &params,
						const QString &pass);
		static void openLocalDatabase(const QString &connName,
						const QString &descriptor);
};

#endif