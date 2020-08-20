/*
 * Author: Antonino Musmeci
 */

#include <cte/client/loginTextEditor.h>
#include <QApplication>
#include <QtCore/QSettings>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("DivideEtImpera");
    QCoreApplication::setOrganizationDomain("divideetimpera.ga"); // choose the domain name from the ones available
    QCoreApplication::setApplicationName("Collaborative Text Editor");
    // setting client values (it should be done with installer)
/*
    QSettings client_default = QSettings(QSettings::SystemScope);
    client_default.beginGroup("client");
    client_default.setValue("hostname", QString("divideetimpera.ga")); // domain name to be chosen
    client_default.setValue("secondhostname", QString("localhost"));
    quint16 port = 8080;
    client_default.setValue("port", port);
    client_default.endGroup();
    client_default.sync();
*/
    loginTextEditor l;
    return a.exec();
}
