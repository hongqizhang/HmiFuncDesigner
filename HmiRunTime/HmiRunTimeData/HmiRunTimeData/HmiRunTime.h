﻿#ifndef HMIRUNTIME_H
#define HMIRUNTIME_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QMutex>
#include "Vendor.h"
#include "RealTimeDB.h"
#include "PortThread.h"


class HmiRunTime : public QObject
{
    Q_OBJECT
public:
    explicit HmiRunTime(QString projectPath, QObject *parent = Q_NULLPTR);
    ~HmiRunTime();
    bool Load(SaveFormat saveFormat);
    bool Unload();
    void Start();
    void Stop();
    void AddPortName(const QString name);
    Vendor *FindVendor(const QString name);
    QJsonObject LoadJsonObjectFromFile(SaveFormat saveFormat, QString f);

signals:

public slots:

protected:
    bool event(QEvent *e);

private:
    // 获取工程名称
    QString getProjectName(const QString &szProjectPath);

private:
    static QString m_sProjectPath;
    QStringList m_listPortName;
    QList<Vendor *> m_VendorList;
    QList<PortThread *> m_listPortThread;
};

extern HmiRunTime *g_pHmiRunTime;

#endif // HMIRUNTIME_H
