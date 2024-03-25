#include <iostream>
#include <string>
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

class Service
{
public:
    Service(const std::string &host, int port, const std::string &queueName)
        : m_host(host), m_port(port), m_queueName(queueName) {}

    void run()
    {
        AMQP::LibBoostAsioHandler handler;
        AMQP::TcpConnection connection(&handler, AMQP::Address(m_host, m_port));
        AMQP::TcpChannel channel(&connection);

        channel.declareQueue(m_queueName);

        // Publish a message to the queue
        std::string message = "Hello, RabbitMQ!";
        channel.publish("", m_queueName, message);

        std::cout << "Service published message: " << message << std::endl;
    }

private:
    std::string m_host;
    int m_port;
    std::string m_queueName;
};

class Consumer
{
public:
    Consumer(const std::string &host, int port, const std::string &queueName)
        : m_host(host), m_port(port), m_queueName(queueName) {}

    void run()
    {
        AMQP::LibBoostAsioHandler handler;
        AMQP::TcpConnection connection(&handler, AMQP::Address(m_host, m_port));
        AMQP::TcpChannel channel(&connection);

        channel.declareQueue(m_queueName);

        // Consume messages from the queue
        channel.consume(m_queueName)
            .onReceived([&](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
                        {
                std::string body(message.body(), message.bodySize());
                std::cout << "Consumer received message: " << body << std::endl;
                channel.ack(deliveryTag); });

        handler.loop();
    }

private:
    std::string m_host;
    int m_port;
    std::string m_queueName;
};

int main()
{
    // RabbitMQ connection settings
    std::string host = "localhost";
    int port = 5672; // Default RabbitMQ port
    std::string queueName = "example_queue";

    // Run the service
    Service service(host, port, queueName);
    service.run();

    // Run the consumer
    Consumer consumer(host, port, queueName);
    consumer.run();

    return 0;
}
