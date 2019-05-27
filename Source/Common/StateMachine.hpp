#pragma once

#include "Precompiled.hpp"

template<typename Type>
class State
{
private:
    // Friend declaration.
    template<typename Type>
    friend class StateMachine;

protected:
    State() :
        m_stateMachine(nullptr)
    {
    }

public:
    // Returns assigned state machine.
    StateMachine<Type>* getStateMachine()
    {
        return m_stateMachine;
    }

protected:
    // State machine transitions.
    virtual bool onStateEnter(Type* newState) = 0;
    virtual bool onStateExit(Type* previousState) = 0;

private:
    // Current state machine assigned.
    StateMachine<Type>* m_stateMachine;
};

template<typename Type>
class StateMachine
{
public:
    StateMachine() :
        m_currentState(nullptr)
    {
    }

    ~StateMachine()
    {
        // Exit current state.
        changeState(nullptr);
    }

    // Changes current state.
    bool changeState(Type* state)
    {
        // Check assumption so casts are safe.
        static_assert(std::is_base_of<State<Type>, Type>::value, "Type is not derived from State<Type>!");

        // Validate current state.
        if(m_currentState)
        {
            ASSERT(m_currentState->m_stateMachine, "Current state does not have reference set to state machine!");
        }

        // Make sure state is not already assigned to another state machine.
        if(state && state->m_stateMachine)
        {
            LOG_ERROR("Attempted to set state that is used by another state machine!");
            return false;
        }

        // Exit current state.
        if(m_currentState)
        {
            if(!((State<Type>*)m_currentState)->onStateExit(state))
                return false;

            m_currentState->m_stateMachine = nullptr;
            m_currentState = nullptr;
        }

        // Enter new state.
        if(state)
        {
            state->m_stateMachine = this;
            if(!((State<Type>*)state)->onStateEnter(m_currentState))
            {
                state->m_stateMachine = nullptr;
                return false;
            }
        }

        // Handle special case when entered state may have immediately switched to another state.
        if(m_currentState != nullptr)
        {
            state->m_stateMachine = nullptr;
        }
        else
        {
            m_currentState = state;
        }

        // Success!
        return true;
    }

    // Returns reference to current state.
    Type* getState()
    {
        return m_currentState;
    }

private:
    // Current state.
    Type* m_currentState;
};
