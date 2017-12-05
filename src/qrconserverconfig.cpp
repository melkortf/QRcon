#include "qrconserverconfig.h"
#include <QtCore>

class QRconServerConfigData : public QSharedData {
public:
    QRconServerConfigData() = default;
    QRconServerConfigData(const QString& hostName, const QString& password, quint16 port) :
        hostName(hostName), password(password), port(port) {}

    QString hostName;
    QString password;
    quint16 port = 27015; // most common source dedicated server port
};

QRconServerConfig::QRconServerConfig() :
    d(new QRconServerConfigData) {}

QRconServerConfig::QRconServerConfig(const QString &hostName, const QString &password, quint16 port) :
    d(new QRconServerConfigData(hostName, password, port)) {}

QRconServerConfig::QRconServerConfig(const QRconServerConfig &other) :
    d(other.d) {}

QRconServerConfig::~QRconServerConfig() {}

QRconServerConfig &QRconServerConfig::operator=(const QRconServerConfig &other)
{
    d = other.d;
    return *this;
}

QString QRconServerConfig::hostName() const
{
    return d->hostName;
}

void QRconServerConfig::setHostName(const QString &hostName)
{
    d->hostName = hostName;
}

QString QRconServerConfig::password() const
{
    return d->password;
}

void QRconServerConfig::setPassword(const QString &password)
{
    d->password = password;
}

quint16 QRconServerConfig::port() const
{
    return d->port;
}

void QRconServerConfig::setPort(quint16 port)
{
    d->port = port;
}

QDataStream &operator<<(QDataStream &out, const QRconServerConfig &config)
{
    out << config.hostName() << config.password() << config.port();
    return out;
}

QDataStream &operator>>(QDataStream &in, QRconServerConfig &config)
{
    QString hostName, password;
    quint16 port;
    in >> hostName;
    in >> password;
    in >> port;

    config.setHostName(hostName);
    config.setPassword(password);
    config.setPort(port);

    return in;
}

static void registerType()
{
    qRegisterMetaTypeStreamOperators<QRconServerConfig>("QRconServerConfig");
}
Q_COREAPP_STARTUP_FUNCTION(registerType)
