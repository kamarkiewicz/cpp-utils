/** Observer pattern impl
 * Based on https://en.wikipedia.org/wiki/Observer_pattern
 *
 * It's not thread-safe, but allows to re/unregister observers during notification
 * (ie. by some observer during update())
 *
 */
#include <algorithm>
#include <list>

class Subject;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(Subject *subject) noexcept = 0;
};

class Subject {
    enum class Status : bool {
        UNREGISTERED,
        REGISTERED
    };
    std::list<std::pair<Observer *, Status>> observerCollection;
public:
    void registerObserver(Observer *observer) {
        auto iter = std::find(std::begin(observerCollection), std::end(observerCollection),
                              std::make_pair(observer, Status::REGISTERED));
        if (iter == observerCollection.end()) {
            observerCollection.emplace_front(observer, Status::REGISTERED);
        }
    }

    void unregisterObserver(Observer *observer) {
        auto iter = std::find(std::begin(observerCollection), std::end(observerCollection),
                              std::make_pair(observer, Status::REGISTERED));
        if (iter != observerCollection.end()) {
            iter->second = Status::UNREGISTERED;
        }
    }

    void notifyObservers() {
        // Notify phase
        for (auto &observer : observerCollection) {
            if (observer.second == Status::REGISTERED) {
                observer.first->update(this);
            }
        }
        // Cleanup phase
        observerCollection.remove_if([](auto &p) {
            return p.second == Status::UNREGISTERED;
        });
    }
};


#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>

TEST_CASE("Just works", "[observer]") {
    // Given
    Subject subject;
    class MockObserverA : public Observer {
    public:
        int updateCalled = 0;
        void update(Subject *subject) noexcept override {
            updateCalled++;
        }
    } a;

    class MockObserverB : public Observer {
    public:
        int updateCalled = 0;
        void update(Subject *subject) noexcept override {
            updateCalled++;
        }
    } b;

    // When
    subject.registerObserver(&a);
    subject.registerObserver(&b);
    subject.notifyObservers();

    // Then
    REQUIRE(a.updateCalled == 1);
    REQUIRE(b.updateCalled == 1);
}

TEST_CASE("Register while notifyObservers", "[observer]") {
    // Given
    Subject subject;
    class MockObserverA : public Observer {
    public:
        int updateCalled = 0;
        void update(Subject *subject) noexcept override {
            updateCalled++;
        }
    } a;

    class MockObserverB : public Observer {
        Observer* m_a;
    public:
        explicit MockObserverB(Observer *pA) : m_a(pA) {}
        int updateCalled = 0;
        void update(Subject *subject) noexcept override {
            updateCalled++;
            subject->registerObserver(m_a); // on next notifyObservers m_a will be called.
            // m_a->update(subject); // MockObserverB can make a decision
                                     // to notify or not m_a while his update() call.
        }
    } b(&a);

    // When
    subject.registerObserver(&b);
    subject.notifyObservers();
    subject.notifyObservers();

    // Then
    REQUIRE(a.updateCalled == 1);
    REQUIRE(b.updateCalled == 2);
}

TEST_CASE("Unregister self while notifyObservers", "[observer]") {
    // Given
    Subject subject;
    class MockObserverA : public Observer {
    public:
        int updateCalled = 0;
        void update(Subject *subject) noexcept override {
            updateCalled++;
            subject->unregisterObserver(this);
        }
    } a;

    // When
    subject.registerObserver(&a);
    subject.notifyObservers();
    subject.notifyObservers();

    // Then
    REQUIRE(a.updateCalled == 1);
}


TEST_CASE("Re-register self while notifyObservers", "[observer]") {
    // Given
    Subject subject;
    class MockObserverA : public Observer {
    public:
        int updateCalled = 0;
        void update(Subject *subject) noexcept override {
            updateCalled++;
            subject->unregisterObserver(this);
            subject->registerObserver(this);
        }
    } a;

    // When
    subject.registerObserver(&a);
    subject.notifyObservers();
    subject.notifyObservers();

    // Then
    REQUIRE(a.updateCalled == 2);
}

TEST_CASE("Re-registering another observer while notifyObservers", "[observer]") {
    // Given
    Subject subject;
    class MockObserverA : public Observer {
    public:
        int updateCalled = 0;
        void update(Subject *subject) noexcept override {
            updateCalled++;
        }
    } a;

    class MockObserverB : public Observer {
        Observer* m_a;
    public:
        explicit MockObserverB(Observer *pA) : m_a(pA) {}
        int updateCalled = 0;
        void update(Subject *subject) noexcept override {
            updateCalled++;
            subject->unregisterObserver(m_a); // current notifyObservers may not call m_a.update(),
                                              // which is a desired behaviour (we can call it now by ourselves).
            subject->registerObserver(m_a);   // on next notifyObservers m_a will be called.
        }
    } b(&a);

    // When
    // NOTICE: notification is performed in reverse order of registration
    subject.registerObserver(&b);
    subject.registerObserver(&a);
    subject.notifyObservers();
    subject.notifyObservers();

    // Then
    REQUIRE(a.updateCalled == 2);
    REQUIRE(b.updateCalled == 2);
}
