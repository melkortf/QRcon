#ifndef QRCONSERVERCONFIG_H
#define QRCONSERVERCONFIG_H

#include "qrconexport.h"
#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>

class QRconServerConfigData;

/**
 * \ingroup QRcon
 * @{
 *
 * The \c QRconServerConfig class provides server configuration that can be
 * used with \ref QRconSession as well as serialized.
 */
class __QRconApi__ QRconServerConfig {
public:
    /**
     * Creates an empty server config. \c QRConSession will always fail
     * to connect with such config.
     */
    QRconServerConfig();

    /**
     * Creates a full server config from the arguments provided.
     */
    QRconServerConfig(const QString& hostName, const QString& password, quint32 port);

    /**
     * Creates a copy of \c other with shared underlying data.
     */
    QRconServerConfig(const QRconServerConfig& other);

    /**
     * Destroys this server config.
     */
    virtual ~QRconServerConfig();

    QRconServerConfig& operator=(const QRconServerConfig& other);

    QString hostName() const;
    void setHostName(const QString& hostName);
    QString password() const;
    void setPassword(const QString& password);
    quint32 port() const;
    void setPort(quint32 port);

private:
    QSharedDataPointer<QRconServerConfigData> d;

}; /** @} */

Q_DECLARE_METATYPE(QRconServerConfig)
Q_DECLARE_TYPEINFO(QRconServerConfig, Q_MOVABLE_TYPE);

__QRconApi__ QDataStream& operator<<(QDataStream& out, const QRconServerConfig& config);
__QRconApi__ QDataStream& operator>>(QDataStream& in, QRconServerConfig& config);

#endif // QRCONSERVERCONFIG_H
