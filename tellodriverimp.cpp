#include "tellodriverimp.h"
#include <exception>

TelloDriverImp::TelloDriverImp()
    : cmd_init(false), telemetry_init(false), stream_init(false)
{
    addr_cmd = std::make_unique<Addr>("192.168.10.1", 8889);
    addr_telemetry = std::make_unique<Addr>("0.0.0.0", 8890);
    addr_stream = std::make_unique<Addr>("0.0.0.0", 11111);
}

void TelloDriverImp::Init()
{
    if(cb_in_cmd && !cmd_init)
    {
        sock_cmd.abort();
        if(!sock_cmd.bind(QHostAddress("0.0.0.0"), addr_cmd->port, QUdpSocket::DontShareAddress))
            throw std::invalid_argument("Can't bind to socket: 0.0.0.0:8889");
        connect(&sock_cmd, &QUdpSocket::readyRead, this, &TelloDriverImp::readCmdAnswers);
        cmd_init = true;
    }

    if(cb_telemetry && !telemetry_init)
    {
        sock_telemetry.abort();
        if(!sock_telemetry.bind(addr_telemetry->ip, addr_telemetry->port, QUdpSocket::DontShareAddress))
            throw std::invalid_argument("Can't bind to socket: 0.0.0.0:8890");
        connect(&sock_telemetry, &QUdpSocket::readyRead, this, &TelloDriverImp::readTelemetry);
        telemetry_init = true;
    }

    if(cb_stream && !stream_init)
    {
        sock_stream.abort();
        if(!sock_stream.bind(addr_stream->ip, addr_stream->port, QUdpSocket::DontShareAddress))
            throw std::invalid_argument("Can't bind to socket: 0.0.0.0:11111");
        connect(&sock_stream, &QUdpSocket::readyRead, this, &TelloDriverImp::readStream);
        stream_init = true;
    }
}

void TelloDriverImp::Execute(const std::string& command)
{
    if(cb_out_cmd)
        cb_out_cmd(command);
    sock_cmd.writeDatagram(command.c_str(), static_cast<std::int64_t>(command.size()), addr_cmd->ip, addr_cmd->port);
}

void TelloDriverImp::readCmdAnswers()
{
    while(sock_cmd.hasPendingDatagrams())
    {
        QNetworkDatagram datagram = sock_cmd.receiveDatagram();
        cb_in_cmd(datagram);
    }
}

void TelloDriverImp::readTelemetry()
{
    while(sock_telemetry.hasPendingDatagrams())
    {
        QNetworkDatagram datagram = sock_telemetry.receiveDatagram();
        cb_telemetry(datagram);
    }
}

void TelloDriverImp::readStream()
{
    while(sock_stream.hasPendingDatagrams())
    {
        QNetworkDatagram datagram = sock_stream.receiveDatagram();
        cb_stream(datagram);
    }
}
