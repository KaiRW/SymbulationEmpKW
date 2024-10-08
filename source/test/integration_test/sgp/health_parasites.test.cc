#include "../../../sgp_mode/SGPWorld.h"
#include "../../../sgp_mode/SGPHost.h"
#include "../../../sgp_mode/SGPWorldSetup.cc"

#include "../../../catch/catch.hpp"

TEST_CASE("Not Tasks Drop", "[kw]"){
    emp::Random random(61);
    SymConfigSGP config;
    TaskSet task_set = LogicTasks;
    SGPWorld world(random, &config, task_set);
    
    //Change settings
    config.UPDATES(7001);

    WHEN("Parasites are present"){
        config.START_MOI(1);

        world.Setup();

        // COULD MAKE THIS A CHECK!!! (won't stop running) OR REQUIRE (STOP RUN)
        auto & host_node = world.GetHostCountDataNode();
        std::cout << host_node.GetTotal() << std::endl;

        world.RunExperiment(false);

        auto & node = world.GetHostTasksDataNode();

        THEN("Host not count between 7000-8500"){
            host_node = world.GetHostCountDataNode();
            std::cout << host_node.GetTotal() << std::endl;

            double current_not_count = node[0].GetTotal();

            REQUIRE(current_not_count >= 7000);
            REQUIRE(current_not_count <= 8500);
        }
    }

    WHEN("No parasites are present"){
        config.START_MOI(0);

        auto & node = world.GetHostTasksDataNode();

        world.Setup();

        auto & host_node = world.GetHostCountDataNode();
        std::cout << host_node.GetTotal() << std::endl;

        world.RunExperiment(false);

        THEN("Host not count below 7000"){
            host_node = world.GetHostCountDataNode();
            std::cout << host_node.GetTotal() << std::endl;

            double current_not_count = node[0].GetTotal();
            REQUIRE(current_not_count <= 7000);
        }
    }

}