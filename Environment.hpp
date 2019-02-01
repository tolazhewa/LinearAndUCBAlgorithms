#ifndef ENVIRONMENT_CLASS
#define ENVIRONMENT_CLASS
#define NUM_SPACE std::setw(5) // Formatting support for printing an array

#include <vector>
#include <iostream>
#include <iomanip>
#include "Arm.hpp"

class Environment{
    private:
        std::vector<Arm> arms; // Collection of arms available
        int optm_prob_index;
        /**
         * Returns argument with the highest probability
         **/
        int get_optm_prob_arg();
        
    public:
        /**
         * Constructor that takes number of arms (n)
         **/
        Environment(int n = 10);
        /**
         * returns the arms vector
         **/
        int get_arms_size();
        
        /**
         * Returns 1 if the choice is optimal (greatest probability), otherwise 0;
         **/
        int is_optimal(int choice);

        /**
         * Prints the arm probabilities of producing reward
         **/
        void print_arm_probs(std::ostream &file = std::cout);

        /**
         * Pull the chosen arm and return reward
         **/
        int pull_chosen_arm(int choice);
};

#endif