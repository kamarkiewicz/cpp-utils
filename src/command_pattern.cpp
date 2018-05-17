/** Command pattern impl
 * Based on https://en.wikipedia.org/wiki/Command_pattern
 */

class Command {
public:
    virtual void execute() = 0;
};

class Invoker {
    Command* m_command;
public:
    explicit Invoker(Command* command) : m_command(command) {}
    void invoke() {
        m_command->execute();
    }
};

class Receiver {
public:
    int count = 0;
    void action() {
       count++;
    }
};

class ConcreteCommand : public Command {
    Receiver* m_receiver;
public:
    explicit ConcreteCommand(Receiver* receiver) : m_receiver(receiver) {}
    void execute() override {
        m_receiver->action();
    }
};


#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Just works", "[command]") {
    // Given
    Receiver receiver;
    ConcreteCommand command(&receiver);
    Invoker invoker(&command);

    // When
    invoker.invoke();

    // Then
    REQUIRE(receiver.count == 1);
}
