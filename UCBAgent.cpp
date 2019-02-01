#include "UCBAgent.hpp"

/**
 * Default constructor
 **/

UCBAgent::UCBAgent(Environment env, std::string l, double conf)
:c(conf)
,optm_chosen(0)
,iter_number(1)
,points(0)
,curr_env(env)
,label(l) {
    int n = env.get_arms_size();
    for (int i = 0; i < n; i++){
        est_arm_reward_prob.push_back(0.5);
        times_arm_pulled.push_back(0);
    }
}

/**
 * returns the cumulative rewards collected
 **/
int UCBAgent::get_points(){ return points; }

/**
 * Prints the agent's probabilities to choose each arm
 **/
void UCBAgent::print_est_arm_reward_probs(std::ostream &file){ 
    file << "Estimated Arm Probs: \t";
    for (auto prob : est_arm_reward_prob)
        file << prob << " " << NUM_SPACE;
    file << std::endl;
}

/**
 * Returns the index of the element with the highest value in a vector containing doubles
 **/
int UCBAgent::argmax(std::vector<double> vec){
    int lrg_index = -1;
    double lrg_val = 0;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > lrg_val) {
            lrg_index = i;
            lrg_val = vec[i];
        }
    }
    return lrg_index;
}

/**
 * Chooses an arm to pull based on the arm probabilities
 **/
int UCBAgent::choose_arm(){ 
    std::vector<double> prob_ucb;
    for(int i = 0; i < est_arm_reward_prob.size(); i++) {
        prob_ucb.push_back(
            est_arm_reward_prob[i] + 
            c * sqrt(log(iter_number)/(times_arm_pulled[i] + 1))
        );
    }
    return argmax(prob_ucb);
}

/**
 * Executes a single round of game and updates arm probabilities
 **/
void UCBAgent::exec_round(){
    int choice, reward;
    double est_choice_prob;

    // Choose an arm
    choice = choose_arm();

    if(choice == -1){
        return;
    }
    
    //Add 1 to optimal chosen variable if the optimal arm is chosen else add 0 (do nothing)
    optm_chosen += curr_env.is_optimal(choice);

    // Increment the number of times this arm has been pulled
    times_arm_pulled[choice]++;

    // Pulls the arm and returns the reward (0 or 1)
    reward = curr_env.pull_chosen_arm(choice);

    // Adds the reward to the total points accumulated
    points += reward;

    // Changes the probabilities based on the UCB Algorithm
    est_arm_reward_prob[choice] = 
        est_arm_reward_prob[choice] + 
        ((reward - est_choice_prob)/iter_number);
    
    // Update the number of iterations
    iter_number++;
}

/**
 * Return the percentage of iterations where reward was received
 **/
double UCBAgent::get_reward_percent(){
    return (double)points / (double)(iter_number-1);
}

/**
 * Return the percentage of iterations where optimal arm was chosen
 **/
double UCBAgent::get_optm_percent(){
    return (double)optm_chosen / (double)(iter_number-1);
}

/**
 * Prints the agent's statistics
 **/
void UCBAgent::print_agent_stats(std::ostream &file){
        file << "-------------------------------------" << label << "---------------------------------------\n";
        file << "Optimal Action Chosen:\t" << std::setw(5) << optm_chosen 
                << "/" << (iter_number-1) << std::endl;

        file << "Percentage:\t" << std::setw(20) 
                << ((double)optm_chosen / (double)(iter_number-1)) * 100 
                << "%" << std::endl;

        file << "Success Rate:\t" << std::setw(13) << points
                << "/" << (iter_number-1) << std::endl;

        file << "Percentage:\t" << std::setw(20) 
                << ((double)points / (double)(iter_number-1)) * 100 
                << "%" << std::endl << std::endl;

        print_est_arm_reward_probs(file);
        curr_env.print_arm_probs(file);
        file << "----------------------------------------------------------------------------------\n\n" << std::endl;
}
/**
 * Resets the agent variables and produces a new set of arm probabilities
 **/
void UCBAgent::change_parameters(Environment &env, double conf){
    curr_env = env;
    points = 0;
    iter_number = 1;
    optm_chosen = 0;
    times_arm_pulled.clear();
    est_arm_reward_prob.clear();

    int n = curr_env.get_arms_size();
    for (int i = 0; i < n; i++){
        times_arm_pulled.push_back(0);
        est_arm_reward_prob.push_back(0.5);
    }

    c = conf;
}
