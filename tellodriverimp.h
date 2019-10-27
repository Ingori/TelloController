#ifndef TELLODRIVERIMP_H
#define TELLODRIVERIMP_H

#include <QString>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <memory>

struct Addr
{
    Addr(const QString& ip_, std::uint16_t port_)
        : ip(ip_), port(port_) {}
    QHostAddress ip;
    std::uint16_t port;
};
using AddrSPtr = std::unique_ptr<Addr>;

using CbTelloOutput = std::function<void(const std::string&)>;
using CbTelloInput = std::function<void(const QNetworkDatagram&)>;

class TelloDriverImp : public QObject
{
    Q_OBJECT

public:
    TelloDriverImp();

    void SetCmdOutCB(CbTelloOutput cb) { cb_out_cmd = cb; }
    void SetCmdInCB(CbTelloInput cb) { cb_in_cmd = cb; }
    void SetTelemetryCB(CbTelloInput cb) { cb_telemetry = cb; }
    void SetStreamCB(CbTelloInput cb) { cb_stream = cb; }
    void Init();

    void Execute(const std::string& command);

private slots:
    void readCmdAnswers();
    void readTelemetry();
    void readStream();

private:
    AddrSPtr addr_cmd;
    QUdpSocket sock_cmd;
    CbTelloInput cb_in_cmd;
    std::atomic_bool cmd_init;

    AddrSPtr addr_telemetry;
    QUdpSocket sock_telemetry;
    CbTelloInput cb_telemetry;
    std::atomic_bool telemetry_init;

    AddrSPtr addr_stream;
    QUdpSocket sock_stream;
    CbTelloInput cb_stream;
    std::atomic_bool stream_init;

    CbTelloOutput cb_out_cmd;
};

#endif // TELLODRIVERIMP_H
