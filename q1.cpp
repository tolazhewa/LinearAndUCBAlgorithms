#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>

#include "Environment.hpp"
#include "Arm.hpp"
#include "UCBAgent.hpp"

#define COL_WIDTH std::setw(10) // Formatting support for printing the statistics
#define COL_WIDTH_2 std::setw(12) // To align numerical values with their heading
#define SIDE_OFFSET std::setw(10) // Side offsets for stats


void print_stats(std::vector<double> cons_val,std::vector<std::vector<double>> ucb_results, 
                int size_of_cons_val, std::string stats_file_name);

/**
 * Main function that executes the program
 **/
int main(){

    //-------------------------------------------------------------------------------------------------------//
    //-------------------------------------------------------------------------------------------------------//

    // Dump and stats file names
    std::string dump_file_name = "q1.out";
    std::string stats_file_name = "q1_stats";

    // Should we collect stats?
    bool collect_stats = true;

    // Should we collect iteration data?
    bool collect_iter_data = true;

    // How often (once every how many iterations) should we print stats of the iteration?
    int print_freq = 100;

    // Number of arms the slot machine should have
    int num_of_arms = 10;

    // Array of considered confidence values
    // To test a wide variety of values, use { 0.01, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 25.0, 50.0, 100.0 };
    std::vector<double> cons_val = { 2.0 };

    // Number of environments used
    int num_of_envs = 100; 

    // Number of Iterations
    int num_of_iters = 5000;

    // The full number of executions will be cons_val.size() * num_of_envs * num_of_iters

    //------------------------------------DO NOT MODIFY BEYOND THIS POINT------------------------------------//
    //-------------------------------------------------------------------------------------------------------//


    srand(time(NULL));

    // Files to write to (dump file and stats file)
    std::ofstream dump_file;

    if (collect_iter_data){
        // Open the dump file
        dump_file.open(dump_file_name, std::ofstream::trunc);

        // Set up the output style preferred
        dump_file << std::fixed;

        //sets the printing double precision to 2 decimal points
        dump_file << std::setprecision(2); 
    }
    
    // Temporary values to use for getting averages and other operations
    double ucb_point_avg, ucb_optm_avg, curr_conf;

    // Environment Variable to represent the current environment
    Environment curr_env;

    // L(r-p) agent
    UCBAgent ucb(curr_env, "UCB");

    // Size of considered values
    int size_of_cons_val = cons_val.size();
    
    // Array containing the results (% optimal arm chosen, % reward collected) for the l(r-i) algorithm
    std::vector<std::vector<double>> ucb_results;
    for(int i = 0; i < size_of_cons_val; i++) {
        std::vector<double> v1;
        ucb_results.push_back(v1);
    }

    // For each conf value
    for (int conf_index = 0; conf_index < size_of_cons_val; conf_index++) {
        // Reset the variables 
        ucb_point_avg = 0,  ucb_optm_avg = 0;

        // Get the current values from our array
        curr_conf = cons_val[conf_index];
        
        // Iterate number of environments indicated times
        for (int env_count = 0; env_count < num_of_envs; env_count++) {
            // Create a new environment with the indicated number of arms
            curr_env = Environment(num_of_arms);

            // Change the parameters of the agents to accomodate for the current configuration
            ucb.change_parameters(curr_env, curr_conf);

            for (int iter_num = 1; iter_num <= num_of_iters; iter_num++) {
                // Execute an iteration (round) for both agents
                ucb.exec_round();

                // Print out once very print_freq number of times
                if (iter_num % print_freq == 0 && collect_iter_data) {
                    ucb.print_agent_stats(dump_file);
                }
            }
            ucb_point_avg += ucb.get_reward_percent();
            ucb_optm_avg += ucb.get_optm_percent();
        }

        // Get the average points percent for L(r-p) (divide by n)
        ucb_point_avg /= num_of_envs; 
        // Get the average percentage of time optimal arm was chosen for L(r-p)
        ucb_optm_avg /= num_of_envs; 

        // Store results in the results arrays
        ucb_results[conf_index].push_back(ucb_optm_avg); 
        ucb_results[conf_index].push_back(ucb_point_avg);
    }
    
    if (collect_iter_data){
        dump_file.close();
    }

    if (collect_stats) {
        print_stats(cons_val, ucb_results, size_of_cons_val, stats_file_name);
    }
    return 0;
}
/**
 * Prints all statistics to file
 **/
void print_stats(std::vector<double> cons_val,std::vector<std::vector<double>> ucb_results, 
                int size_of_cons_val, std::string stats_file_name){
    // Open the stats file
    std::ofstream stats_file;
    stats_file.open(stats_file_name, std::ofstream::trunc);

    // Set up the output style preferred
    stats_file << std::fixed;
    
    //sets the printing double precision to 2 decimal points
    stats_file << std::setprecision(2); 

    // Prints the header for the UCB algorithm
    stats_file << "\n\nUCB Statistics\n\n";
    stats_file << SIDE_OFFSET << "conf" << COL_WIDTH_2 << "% Optimal" << COL_WIDTH
              << "% Reward" << std::endl;
    stats_file << "----------------------------------------------------------------------------------------------------" << std::endl;
    // Print all the values
    for (int conf_index = 0; conf_index < size_of_cons_val; conf_index++) {
        stats_file << SIDE_OFFSET << cons_val[conf_index]
                  << COL_WIDTH << ucb_results[conf_index][0] * 100 << "%"
                  << COL_WIDTH << ucb_results[conf_index][1] * 100 << "%" << std::endl;
    }
    stats_file << "----------------------------------------------------------------------------------------------------" << std::endl;
    stats_file.close();
}