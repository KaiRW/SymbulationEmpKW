#include "../../../sgp_mode/SGPWorld.h"
#include "../../../sgp_mode/SGPHost.h"
#include "../../../sgp_mode/SGPWorldSetup.cc"

#include "../../../catch/catch.hpp"

/*
Turn spatial structure on: GRID == 1
Look at proposal again for main takeway,
    high level results for with/without parasites from proposal
*/

TEST_CASE("Not Tasks Drop", "[kw]"){
    SymConfigSGP config;
    emp::Random random(10);

    //std::cout << config.SEED() << std::endl;
    TaskSet task_set = LogicTasks;
    
    //Change settings
    config.UPDATES(7001);
    config.DATA_INT(50);
    config.VERTICAL_TRANSMISSION(0.0);
    config.LIMITED_RES_TOTAL(10);
    config.LIMITED_RES_INFLOW(500);
    config.HOST_REPRO_RES(100);
    config.SYM_HORIZ_TRANS_RES(10);
    config.THREAD_COUNT(1);
    config.DONATION_STEAL_INST(0);
    config.LIMITED_TASK_RESET_INTERVAL(20);

    //config.Write(std::cout);

    WHEN("Parasites are present"){
        SGPWorld world(random, &config, task_set);

        config.START_MOI(1);

        world.Setup();

        // COULD MAKE THIS A CHECK!!! (won't stop running) OR REQUIRE (STOP RUN)
        auto & host_node = world.GetHostCountDataNode();
        //std::cout << "Start of parasites present: " << host_node.GetTotal() << std::endl;

        world.RunExperiment(false);

        auto & node = world.GetHostTasksDataNode();

        THEN("Host not count between 7000-8500"){
            host_node = world.GetHostCountDataNode();
            //std::cout << "End of parasites present: " << host_node.GetTotal() << std::endl;

            double current_not_count = node[0].GetTotal();
            std::cout << "End of parasites present: " << current_not_count << std::endl;

            REQUIRE(current_not_count <= 7000);
        }
    }

    WHEN("No parasites are present"){
        SGPWorld world(random, &config, task_set);
        config.START_MOI(0);

        auto & node = world.GetHostTasksDataNode();

        world.Setup();

        auto & host_node = world.GetHostCountDataNode();
        //std::cout << "Start of NO parasites present: " << host_node.GetTotal() << std::endl;

        world.RunExperiment(false);

        THEN("Host not count below 7000"){
            host_node = world.GetHostCountDataNode();
            //std::cout << "End of NO parasites present: " << host_node.GetTotal() << std::endl;
            
            double current_not_count = node[0].GetTotal();
            std::cout << "End of NO parasites present: " << current_not_count << std::endl;

            REQUIRE(current_not_count >= 7000);
            //REQUIRE(current_not_count <= 8500);
        }
    }

}