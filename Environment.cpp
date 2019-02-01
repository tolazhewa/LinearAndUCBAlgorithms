#include "Environment.hpp"

/**
 * Constructor that takes number of arms (n)
 **/
Environment::Environment(int n) {
    for(int i = 0; i < n; i++) {
        arms.push_back(Arm());
    }
    optm_prob_index = get_optm_prob_arg();
} 

/**
 * Returns argument with the highest probability
 **/
int Environment::get_optm_prob_arg() {
    int optm_index = -1;
    double optm_val = 0;
    for (int i = 0; i < arms.size(); i++) {
        if (arms[i].get_prob() > optm_val) {
            optm_index = i;
            optm_val = arms[i].get_prob();
        }
    }
    return optm_index;
}

/**
 * returns the arms vector
 **/
int Environment::get_arms_size(){ return arms.size(); }

/**
 * Returns 1 if the choice is optimal (greatest probability), otherwise 0;
 **/
int Environment::is_optimal(int choice) { return (optm_prob_index == choice) ? 1 : 0; }

/**
 * Prints the arm probabilities of producing reward
 **/
void Environment::print_arm_probs(std::ostream &file){
    file << "Arm Success Probs: \t\t";
    for (auto arm : arms)
        file << arm.get_prob() << " " << NUM_SPACE;
    file << std::endl;
}

/**
 * Pull the chosen arm and return reward
 **/
int Environment::pull_chosen_arm(int choice){
    return arms[choice].pull_arm();
}