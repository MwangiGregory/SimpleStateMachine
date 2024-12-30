#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

template <typename EventType>
class StateMachine;

template <typename EventType>
class State
{
public:
    virtual ~State() = default;

    void set_context(StateMachine<EventType> *context)
    {
        if (context != nullptr)
            this->m_controller = context;
    }

    void on_entry() { _on_entry(); }
    void on_exit() { _on_exit(); }
    void handle_event(const EventType event)
    {
        if (_check_guard_condition(event) == false)
            return;
        _handle_event(event);
    }
    void run() { _run(); }

protected:
    State() = default;

    StateMachine<EventType> *m_controller;

private:
    virtual void _on_entry() {}
    virtual void _on_exit() {}
    virtual void _handle_event(const EventType event) = 0;
    virtual void _run() {}
    virtual bool _check_guard_condition(const EventType event) { return true; }
};

template <typename EventType>
class StateMachine
{
public:
    StateMachine(State<EventType> *initial_state) : m_state{initial_state}
    {
        if (m_state != nullptr)
        {
            m_state->set_context(this);
            m_state->on_entry();
        }
    }

    void TransitionTo(State<EventType> *next_state)
    {
        if (next_state != nullptr && m_state != next_state)
        {
            m_state->on_exit();
            next_state->set_context(this);
            next_state->on_entry();
            this->m_state = next_state;
        }
    }

    void FeedEvent(const EventType event) { _feed_event(event); }
    void run() { m_state->run(); }

private:
    virtual void _feed_event(const EventType event) { this->m_state->handle_event(event); }

    State<EventType> *m_state{nullptr};
};

#endif // STATE_MACHINE_H

