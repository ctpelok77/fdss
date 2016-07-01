#include "landmark_factory_rpg_exhaust.h"

#include "landmark_graph.h"

#include "../option_parser.h"
#include "../plugin.h"
#include "../task_proxy.h"

#include <vector>
using namespace std;

namespace landmarks {
/* Problem: We don't get any orders here. (All we have is the reasonable orders
   that are inferred later.) It's thus best to combine this landmark generation
   method with others, don't use it by itself. */

LandmarkFactoryRpgExhaust::LandmarkFactoryRpgExhaust(const Options &opts)
    : LandmarkFactory(opts) {
}

void LandmarkFactoryRpgExhaust::generate_landmarks(
    const TaskProxy &task_proxy, Exploration &exploration) {
    cout << "Generating landmarks by testing all facts with RPG method" << endl;

    // insert goal landmarks and mark them as goals
    for (FactProxy goal : task_proxy.get_goals()) {
        LandmarkNode *lmp = &lm_graph->landmark_add_simple(
            make_pair(goal.get_variable().get_id(), goal.get_value()));
        lmp->in_goal = true;
    }
    // test all other possible facts
    State initial_state = task_proxy.get_initial_state();
    VariablesProxy variables = task_proxy.get_variables();
    for (VariableProxy var : variables) {
        for (int val = 0; val < var.get_domain_size(); ++val) {
            const pair<int, int> lm(var.get_id(), val);
            if (!lm_graph->simple_landmark_exists(lm)) {
                LandmarkNode *new_lm = &lm_graph->landmark_add_simple(lm);
                if (initial_state[lm.first].get_value() != lm.second && relaxed_task_solvable(task_proxy, exploration, true, new_lm)) {
                    assert(lm_graph->landmark_exists(lm));
                    LandmarkNode *node;
                    if (lm_graph->simple_landmark_exists(lm))
                        node = &lm_graph->get_simple_lm_node(lm);
                    else
                        node = &lm_graph->get_disj_lm_node(lm);
                    lm_graph->rm_landmark_node(node);
                }
            }
        }
    }
}

bool LandmarkFactoryRpgExhaust::supports_conditional_effects() const {
    return false;
}

static LandmarkFactory *_parse(OptionParser &parser) {
    parser.document_synopsis(
        "Exhaustive Landmarks",
        "Exhaustively checks for each fact if it is a landmark."
        "This check is done using relaxed planning.");
    parser.document_note(
        "Relevant options",
        "reasonable_orders, only_causal_landmarks");
    _add_options_to_parser(parser);

    Options opts = parser.parse();

    parser.document_language_support("conditional_effects",
                                     "ignored, i.e. not supported");

    if (parser.dry_run()) {
        return nullptr;
    } else {
        return new LandmarkFactoryRpgExhaust(opts);
    }
}

static Plugin<LandmarkFactory> _plugin(
    "lm_exhaust", _parse);
}
