#ifndef QRCONSERVERCONFIG_H
#define QRCONSERVERCONFIG_H

#include "qrcon_export.h"
#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>

class QRconServerConfigData;

/**
 * \ingroup QRcon
 * @{
 *
 * The \c QRconServerConfig class provides server configuration that can be
 * used with \ref QRconSession as well as serialized.
 */
class QRCON_EXPORT QRconServerConfig {
public:
    /**
     * Creates an empty server config. \c QRConSession will always fail
     * to connect with such config.
     */
    QRconServerConfig();

    /**
     * Creates a full server config from the arguments provided.
     */
    QRconServerConfig(const QString& hostName, const QString& password, quint16 port);

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
    quint16 port() const;
    void setPort(quint16 port);

private:
    QSharedDataPointer<QRconServerConfigData> d;

}; /** @} */

Q_DECLARE_METATYPE(QRconServerConfig)
Q_DECLARE_TYPEINFO(QRconServerConfig, Q_MOVABLE_TYPE);

QRCON_EXPORT QDataStream& operator<<(QDataStream& out, const QRconServerConfig& config);
QRCON_EXPORT QDataStream& operator>>(QDataStream& in, QRconServerConfig& config);

#endif // QRCONSERVERCONFIG_H
