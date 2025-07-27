/*
 * Copyright 2025 Oleksandr Geronime
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 */

#pragma once

#include <typeinfo>

#define DEFINE_STATE_ACTIONS(StateName, EnterMethod, ExitMethod) \
    template <>                                                  \
    inline void CFSM::state_actions<CFSM::StateName>::enter(     \
        CFSM::data_model &m)                                     \
    {                                                            \
        m._action_handler->EnterMethod(m);                       \
    }                                                            \
                                                                 \
    template <>                                                  \
    inline void CFSM::state_actions<CFSM::StateName>::exit(      \
        CFSM::data_model &m)                                     \
    {                                                            \
        m._action_handler->ExitMethod(m);                        \
    }

namespace serp
{

    class IActions;

    class CFSM
    {
    public:
        struct data_model
        {
            friend class CFSM;
            data_model() : _action_handler(nullptr) {}

        private:
            IActions *_action_handler;
        } _model;

    public:
        class state
        {
        public:
            virtual state *trigger_CANCEL(CFSM &) { return nullptr; }
            virtual state *trigger_FINISH(CFSM &) { return nullptr; }
            virtual state *trigger_FINISHED(CFSM &) { return nullptr; }
            virtual state *trigger_RUN(CFSM &) { return nullptr; }
            virtual state *unconditional(CFSM &) { return nullptr; }
            virtual state *unconditional_async(CFSM &) { return nullptr; }
            virtual state *initial(CFSM &) { return nullptr; }

            template <class T>
            void enter(data_model &, ...) {}

            template <class T>
            void exit(data_model &, ...) {}

            virtual void exit(data_model &, const std::type_info &) {}

            virtual ~state() = default;
        };

        state *_cur_state = nullptr;
        using event = state *(state::*)(CFSM &);

    private:
        class CPAsyncEventTransitionData
        {
        public:
            CPAsyncEventTransitionData(CFSM &fsm, event trigger) : _fsm(fsm), _trigger(trigger) {}
            inline void dispatch() { _fsm.dispatch(_trigger); }

        private:
            CFSM &_fsm;
            event _trigger;
        };

        class CPAsyncTriggerInvoker
        {
        public:
            virtual ~CPAsyncTriggerInvoker() = default;
            explicit CPAsyncTriggerInvoker(CFSM &fsm) : _fsm(fsm) {}
            static inline state *makeAsyncCall(event) { return nullptr; }
            static inline state *makeAsyncUCall(event) { return reinterpret_cast<state *>(0xFFFFFF); }

        protected:
            virtual void processEvent() {}

        private:
            CFSM &_fsm;
        } _async_trigger_invoker;

    public:
        template <class C>
        class state_actions
        {
        protected:
            static void enter(data_model &) {}
            static void exit(data_model &) {}
        };

        template <class C, class P>
        class composite : public P, public state_actions<C>
        {
            virtual state *initial(CFSM &) { return nullptr; }

        public:
            template <class T>
            static void enter(data_model &, composite *) {}
            template <class T>
            static void enter(data_model &m, ...)
            {
                P::template enter<T>(m, (T *)nullptr);
                state_actions<C>::enter(m);
            }
            template <class T>
            static void exit(data_model &, composite *) {}
            template <class T>
            static void exit(data_model &m, ...)
            {
                state_actions<C>::exit(m);
                P::template exit<T>(m, (T *)nullptr);
            }
            virtual void exit(data_model &m, const std::type_info &sti)
            {
                if (typeid(C) == sti)
                    return;
                state_actions<C>::exit(m);
                P::exit(m, sti);
            }
        };

        class no_state
        {
        };

        enum transition_type
        {
            external,
            internal
        };

        template <int index, event E, class S, class D0 = no_state>
        class transition_actions
        {
        protected:
            static void enter(data_model &) {}
            static bool condition(data_model &) { return true; }
        };

        template <int index, event E, class S, class D = no_state, transition_type T = external>
        class transition : public transition_actions<index, E, S, D>
        {
        public:
            state *operator()(S *s, D &d, CFSM &sc)
            {
                if (!transition_actions<index, E, S, D>::condition(sc._model))
                    return nullptr;
                s->exit(sc._model, typeid(S));
                s->template exit<D>(sc._model);
                transition_actions<index, E, S, D>::enter(sc._model);
                d.template enter<S>(sc._model);
                return &d;
            }
        };

        template <int index, event E, class S, class D>
        class transition<index, E, S, D, internal> : public transition_actions<index, E, S, D>
        {
        public:
            state *operator()(S *s, D &d, CFSM &sc)
            {
                if (!transition_actions<index, E, S, D>::condition(sc._model))
                    return nullptr;
                s->exit(sc._model, typeid(S));
                s->template exit<D>(sc._model, (D *)nullptr);
                transition_actions<index, E, S, D>::enter(sc._model);
                d.template enter<S>(sc._model, (S *)nullptr);
                return &d;
            }
        };

        template <int index, event E, class S>
        class transition<index, E, S, no_state> : public transition_actions<index, E, S, no_state>
        {
        public:
            S *operator()(S *s, CFSM &sc)
            {
                if (!transition_actions<index, E, S, no_state>::condition(sc._model))
                    return nullptr;
                transition_actions<index, E, S, no_state>::enter(sc._model);
                return s;
            }
        };

    private:
        bool dispatch_event(event e)
        {
            state *next_state;
            if ((next_state = (_cur_state->*e)(*this)))
                _cur_state = next_state;
            return next_state != nullptr;
        }

    public:
        void dispatch(event e)
        {
            bool cont = dispatch_event(e);
            while (cont)
            {
                cont = dispatch_event(&state::initial);
                if (cont)
                    continue;

                if (((_cur_state->*(&state::unconditional_async))(*this)) == reinterpret_cast<state *>(0xFFFFFF))
                    continue;

                cont = dispatch_event(&state::unconditional);
                if (cont)
                    continue;

                break;
            }
        }

        explicit CFSM(IActions *pActionHandler)
            : _cur_state(&_state_scxml), _async_trigger_invoker(*this)
        {
            _model._action_handler = pActionHandler;
        }

        void init() { dispatch(&state::initial); }

        class scxml : public composite<scxml, state>
        {
            state *initial(CFSM &sc)
            {
                return transition<0, &state::initial, scxml, state_Init, internal>()(this, sc._state_init, sc);
            }
        } _state_scxml;

        class state_Init : public composite<state_Init, scxml>
        {
            state *unconditional(CFSM &sc) override
            {
                return transition<1, &state::unconditional, state_Init, state_Idle>()(this, sc._state_idle, sc);
            }
        } _state_init;

        class state_Running : public composite<state_Running, scxml>
        {
            state *trigger_CANCEL(CFSM &sc) override
            {
                return transition<2, &state::trigger_CANCEL, state_Running, state_Aborting>()(this, sc._state_aborting, sc);
            }
            state *trigger_FINISH(CFSM &sc) override
            {
                return transition<3, &state::trigger_FINISH, state_Running, state_postProcessing>()(this, sc._state_post_processing, sc);
            }
        } _state_running;

        class state_Finished : public composite<state_Finished, scxml>
        {
        } _state_finished;

        class state_Aborting : public composite<state_Aborting, scxml>
        {
            state *trigger_FINISH(CFSM &sc) override
            {
                return transition<4, &state::trigger_FINISH, state_Aborting, state_postProcessing>()(this, sc._state_post_processing, sc);
            }
        } _state_aborting;

        class state_Idle : public composite<state_Idle, scxml>
        {
            state *trigger_CANCEL(CFSM &sc) override
            {
                return transition<6, &state::trigger_CANCEL, state_Idle, state_Canceling>()(this, sc._state_canceling, sc);
            }
            state *trigger_RUN(CFSM &sc) override
            {
                return transition<5, &state::trigger_RUN, state_Idle, state_Running>()(this, sc._state_running, sc);
            }
        } _state_idle;

        class state_Canceling : public composite<state_Canceling, scxml>
        {
            state *trigger_FINISH(CFSM &sc) override
            {
                return transition<7, &state::trigger_FINISH, state_Canceling, state_postProcessing>()(this, sc._state_post_processing, sc);
            }
        } _state_canceling;

        class state_postProcessing : public composite<state_postProcessing, scxml>
        {
            state *trigger_FINISHED(CFSM &sc) override
            {
                return transition<8, &state::trigger_FINISHED, state_postProcessing, state_Finished>()(this, sc._state_finished, sc);
            }
        } _state_post_processing;
    };

    // Interface for FSM state handlers
    class IActions
    {
    public:
        virtual void stateInitEnter(CFSM::data_model &m) = 0;
        virtual void stateInitExit(CFSM::data_model &m) = 0;
        virtual void stateRunningEnter(CFSM::data_model &m) = 0;
        virtual void stateRunningExit(CFSM::data_model &m) = 0;
        virtual void stateFinishedEnter(CFSM::data_model &m) = 0;
        virtual void stateFinishedExit(CFSM::data_model &m) = 0;
        virtual void stateAbortingEnter(CFSM::data_model &m) = 0;
        virtual void stateAbortingExit(CFSM::data_model &m) = 0;
        virtual void stateIdleEnter(CFSM::data_model &m) = 0;
        virtual void stateIdleExit(CFSM::data_model &m) = 0;
        virtual void stateCancelingEnter(CFSM::data_model &m) = 0;
        virtual void stateCancelingExit(CFSM::data_model &m) = 0;
        virtual void statePostProcessingEnter(CFSM::data_model &m) = 0;
        virtual void statePostProcessingExit(CFSM::data_model &m) = 0;

        virtual void onReset(CFSM::data_model &m) = 0;

    protected:
        virtual ~IActions() = default;
    };

    // Default empty implementation
    class CActionsDefault : public IActions
    {
    public:
        void stateInitEnter(CFSM::data_model &) override {}
        void stateInitExit(CFSM::data_model &) override {}
        void stateRunningEnter(CFSM::data_model &) override {}
        void stateRunningExit(CFSM::data_model &) override {}
        void stateFinishedEnter(CFSM::data_model &) override {}
        void stateFinishedExit(CFSM::data_model &) override {}
        void stateAbortingEnter(CFSM::data_model &) override {}
        void stateAbortingExit(CFSM::data_model &) override {}
        void stateIdleEnter(CFSM::data_model &) override {}
        void stateIdleExit(CFSM::data_model &) override {}
        void stateCancelingEnter(CFSM::data_model &) override {}
        void stateCancelingExit(CFSM::data_model &) override {}
        void statePostProcessingEnter(CFSM::data_model &) override {}
        void statePostProcessingExit(CFSM::data_model &) override {}

        void onReset(CFSM::data_model &) override {}

    protected:
        ~CActionsDefault() override = default;
    };

    // Specializations for state entry/exit actions that call the actionHandler:
    DEFINE_STATE_ACTIONS(state_Init, stateInitEnter, stateInitExit)
    DEFINE_STATE_ACTIONS(state_Running, stateRunningEnter, stateRunningExit)
    DEFINE_STATE_ACTIONS(state_Finished, stateFinishedEnter, stateFinishedExit)
    DEFINE_STATE_ACTIONS(state_Aborting, stateAbortingEnter, stateAbortingExit)
    DEFINE_STATE_ACTIONS(state_Idle, stateIdleEnter, stateIdleExit)
    DEFINE_STATE_ACTIONS(state_Canceling, stateCancelingEnter, stateCancelingExit)
    DEFINE_STATE_ACTIONS(state_postProcessing, statePostProcessingEnter, statePostProcessingExit)

}