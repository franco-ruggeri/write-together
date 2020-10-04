/*
 * Authors: Antonino Musmeci, Franco Ruggeri
 */

#include <cte/network/network_utility.h>
#include <QtCore/QRegExp>

namespace cte {
    static QRegExp ip_address_regexp("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)[.]){3}"
                             "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

    bool check_ip_address(const QString& ip_address) {
        return ip_address_regexp.exactMatch(ip_address);
    }
}