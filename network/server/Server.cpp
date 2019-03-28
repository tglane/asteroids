#include <QApplication>
#include "TcpServer.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    TcpServer server(argv[1], nullptr);
    return app.exec();
}
