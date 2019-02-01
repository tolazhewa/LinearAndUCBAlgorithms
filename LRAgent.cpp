#include "LRAgent.hpp"

/**
 * Default constructor
 **/
LRAgent::LRAgent(Environment env, std::string l, double beta, double alpha)
:alpha(alpha)
,beta(beta)
,optm_chosen(0)
,iter_number(1)
,points(0)
,curr_env(env)
,label(l) {
    int n = env.get_arms_size();
    for (int i = 0; i < n; i++)
        arm_probs.push_back(1.0/n);
}

/**
 * returns the cumulative rewards collected
 **/
int LRAgent::get_points(){ return points; }

/**
 * Prints the agent's probabilities to choose each arm
 **/
void LRAgent::print_arm_sel_probs(std::ostream &file){ 
    file << "Agent Choices Probs: \t";
    for (auto prob : arm_probs)
        file << prob << " " << NUM_SPACE;
    file << std::endl;
}

/**
 * Chooses an arm to pull based on the arm probabilities
 **/
int LRAgent::choose_arm(){ 
    double num = rand()/double(RAND_MAX);
    double total = 0.0;
    for(int i = 0; i < arm_probs.size(); i++){
        total += arm_probs[i];
        if (total >= num)
            return i;
    }
    return -1;
}

/**
 * Executes a single round of game and updates arm probabilities
 **/
void LRAgent::exec_round(){
    int choice, reward;
    double est_choice_prob;

    // Choose an arm
    choice = choose_arm();

    if(choice == -1){
        return;
    }
    
    //Add 1 to optimal chosen variable if the optimal arm is chosen else add 0 (do nothing)
    optm_chosen += curr_env.is_optimal(choice);

    // Pulls the arm and returns the reward (0 or 1)
    reward = curr_env.pull_chosen_arm(choice);

    // Adds the reward to the total points accumulated
    points += reward;

    // Changes the probabilities based on the L(r-p)/L(r-i) algorithms
    if(reward == 1){
        arm_probs[choice] = arm_probs[choice] + alpha * (1.0 - arm_probs[choice]);
        for(int i = 0; i < arm_probs.size(); i++)
            if (i != choice)
                arm_probs[i] = (1.0 - alpha) * arm_probs[i];
    } else {
        arm_probs[choice] = (1.0 - beta) * arm_probs[choice];
        for(int i = 0; i < arm_probs.size(); i++)
            if (i != choice)
                arm_probs[i] = (beta/((double)arm_probs.size()-1.0)) + (1.0 - beta) * arm_probs[i];
    }
    iter_number++;
}

/**
 * Return the percentage of iterations where reward was received
 **/
double LRAgent::get_reward_percent(){
    return (double)points / (double)iter_number;
}

/**
 * Return the percentage of iterations where optimal arm was chosen
 **/
double LRAgent::get_optm_percent(){
    return (double)optm_chosen / (double)iter_number;
}

/**
 * Prints the agent's statistics
 **/
void LRAgent::print_agent_stats(std::ostream &file){
        file << "-------------------------------------" << label << "---------------------------------------\n";
        file << "Optimal Action Chosen:\t" << std::setw(5) << optm_chosen 
                << "/" << iter_number << std::endl;

        file << "Percentage:\t" << std::setw(20) 
                << ((double)optm_chosen / (double)iter_number) * 100 
                << "%" << std::endl;

        file << "Success Rate:\t" << std::setw(13) << points
                << "/" << iter_number << std::endl;

        file << "Percentage:\t" << std::setw(20) 
                << ((double)points / (double)iter_number) * 100 
                << "%" << std::endl << std::endl;

        print_arm_sel_probs(file);
        curr_env.print_arm_probs(file);
        file << "----------------------------------------------------------------------------------\n\n" << std::endl;
}
/**
 * Resets the agent variables and produces a new set of arm probabilities
 **/
void LRAgent::change_parameters(Environment &env, double a, double b){
    curr_env = env;
    points = 0;
    iter_number = 0;
    optm_chosen = 0;
    arm_probs.clear();

    int n = curr_env.get_arms_size();
    for (int i = 0; i < n; i++){
        arm_probs.push_back(1.0/n);
    }
    
    alpha = a;
    beta = b;
}
