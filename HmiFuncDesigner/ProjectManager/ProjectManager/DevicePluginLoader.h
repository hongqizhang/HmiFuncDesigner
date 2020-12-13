#ifndef DEVICEPLUGINLOADER_H
#define DEVICEPLUGINLOADER_H

#include <QString>
#include <QObject>

class IDevicePlugin;


class DevicePluginLoader : public QObject
{
    Q_OBJECT
public:
    static DevicePluginLoader *getInstance() {
        static DevicePluginLoader instance;
        return &instance;
    }

    IDevicePlugin *getPluginObject(const QString &szPluginName);

private:
    DevicePluginLoader(QObject* parent = nullptr);
    ~DevicePluginLoader();

};

#endif // DEVICEPLUGINLOADER_H
