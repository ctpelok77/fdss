#ifndef DELEGATING_TASK_H
#define DELEGATING_TASK_H

#include "abstract_task.h"

class DelegatingTask : public AbstractTask {
protected:
    const AbstractTask &parent;
public:
    explicit DelegatingTask(const AbstractTask &parent_);
    virtual ~DelegatingTask();

    virtual int get_num_variables() const override;
    virtual int get_variable_domain_size(int var) const override;

    virtual int get_operator_cost(int index, bool is_axiom) const override;
    virtual const std::string &get_operator_name(int index, bool is_axiom) const override;
    virtual int get_num_operators() const override;
    virtual int get_num_operator_preconditions(int index, bool is_axiom) const override;
    virtual std::pair<int, int> get_operator_precondition(
        int op_index, int fact_index, bool is_axiom) const override;
    virtual int get_num_operator_effects(int op_index, bool is_axiom) const override;
    virtual int get_num_operator_effect_conditions(
        int op_index, int eff_index, bool is_axiom) const override;
    virtual std::pair<int, int> get_operator_effect_condition(
        int op_index, int eff_index, int cond_index, bool is_axiom) const override;
    virtual std::pair<int, int> get_operator_effect(
        int op_index, int eff_index, bool is_axiom) const override;
    virtual const GlobalOperator *get_global_operator(int index, bool is_axiom) const override;

    virtual int get_num_axioms() const override;

    virtual int get_num_goals() const override;
    virtual std::pair<int, int> get_goal_fact(int index) const override;

    virtual std::vector<int> get_state_values(const GlobalState &global_state) const override;
};

#endif
