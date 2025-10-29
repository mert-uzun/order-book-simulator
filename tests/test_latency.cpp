#include <gtest/gtest.h>
#include <iostream>
#include "../include/LatencyQueue.h"

/**
    ============================================================
    TEST 1: DefaultInitialization
    ============================================================
    PURPOSE: Verify queue starts empty with correct default latency boundaries
    ============================================================
*/
TEST(LatencyQueueTest, DefaultInitialization) {
    LatencyQueue latency_queue;

    EXPECT_TRUE(latency_queue.is_empty())
        << "Queue should be empty initially.";

    EXPECT_EQ(latency_queue.get_latency_boundaries().order_send_min, 50)
        << "Order send min should be 50 initially. Result: " << latency_queue.get_latency_boundaries().order_send_min << std::endl;
    
    EXPECT_EQ(latency_queue.get_latency_boundaries().order_send_max, 200)
        << "Order send max should be 200 initially. Result: " << latency_queue.get_latency_boundaries().order_send_max << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().cancel_min, 30)
        << "Cancel min should be 30 initially. Result: " << latency_queue.get_latency_boundaries().cancel_min << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().cancel_max, 150)
        << "Cancel max should be 150 initially. Result: " << latency_queue.get_latency_boundaries().cancel_max << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().modify_min, 40)
        << "Modify min should be 40 initially. Result: " << latency_queue.get_latency_boundaries().modify_min << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().modify_max, 180)
        << "Modify max should be 180 initially. Result: " << latency_queue.get_latency_boundaries().modify_max << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().acknowledge_fill_min, 100)
        << "Acknowledge fill min should be 100 initially. Result: " << latency_queue.get_latency_boundaries().acknowledge_fill_min << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().acknowledge_fill_max, 400)
        << "Acknowledge fill max should be 400 initially. Result: " << latency_queue.get_latency_boundaries().acknowledge_fill_max << std::endl;
        
    EXPECT_EQ(latency_queue.get_latency_boundaries().market_update_min, 50)
        << "Market update min should be 50 initially. Result: " << latency_queue.get_latency_boundaries().market_update_min << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().market_update_max, 150)
        << "Market update max should be 150 initially. Result: " << latency_queue.get_latency_boundaries().market_update_max << std::endl;
}

/**
    ============================================================
    TEST 2: CustomLatencyProfileSetting
    ============================================================
    PURPOSE: Test setting custom latency boundaries works with limitations
    ============================================================
*/
TEST(LatencyQueueTest, CustomLatencyProfileSetting) {
    LatencyQueue latency_queue;

    // Set the custom values with invalid order_send_min and order_send_max.
    EXPECT_NO_THROW(latency_queue.reset_latency_profile(100, 20, 40, 150, 50, 180, 120, 450, 60, 160))
        << "No exceptions should be thrown even some of the given values are not valid.";

    // Since given min and max for order send are not logical (100 !<= 20), their values should not be updated.
    EXPECT_EQ(latency_queue.get_latency_boundaries().order_send_min, 50)
        << "Order send min should be 50 initially. Result: " << latency_queue.get_latency_boundaries().order_send_min << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().order_send_max, 200)
        << "Order send max should be 200 initially. Result: " << latency_queue.get_latency_boundaries().order_send_max << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().cancel_min, 40)
        << "Cancel min should be 30 initially. Result: " << latency_queue.get_latency_boundaries().cancel_min << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().cancel_max, 150)
        << "Cancel max should be 150 initially. Result: " << latency_queue.get_latency_boundaries().cancel_max << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().modify_min, 50)
        << "Modify min should be 40 initially. Result: " << latency_queue.get_latency_boundaries().modify_min << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().modify_max, 180)
        << "Modify max should be 180 initially. Result: " << latency_queue.get_latency_boundaries().modify_max << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().acknowledge_fill_min, 120)
        << "Acknowledge fill min should be 100 initially. Result: " << latency_queue.get_latency_boundaries().acknowledge_fill_min << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().acknowledge_fill_max, 450)
        << "Acknowledge fill max should be 400 initially. Result: " << latency_queue.get_latency_boundaries().acknowledge_fill_max << std::endl;
        
    EXPECT_EQ(latency_queue.get_latency_boundaries().market_update_min, 60)
        << "Market update min should be 50 initially. Result: " << latency_queue.get_latency_boundaries().market_update_min << std::endl;

    EXPECT_EQ(latency_queue.get_latency_boundaries().market_update_max, 160)
        << "Market update max should be 150 initially. Result: " << latency_queue.get_latency_boundaries().market_update_max << std::endl;


    
}

/**
    ============================================================
    TEST 3: LatencyWithinBounds
    ============================================================
    PURPOSE: Verify computed latency is within min/max range using fixed boundaries
    ============================================================
*/
TEST(LatencyQueueTest, LatencyWithinBounds) {
    
}

/**
    ============================================================
    TEST 4: SingleEventProcessing
    ============================================================
    PURPOSE: Schedule one event, process it, verify callback executes at correct time
    ============================================================
*/
TEST(LatencyQueueTest, SingleEventProcessing) {
    
}

/**
    ============================================================
    TEST 5: MultipleEventsInOrder
    ============================================================
    PURPOSE: Schedule multiple events at different times, verify they execute in chronological order
    ============================================================
*/
TEST(LatencyQueueTest, MultipleEventsInOrder) {
    
}

/**
    ============================================================
    TEST 6: ProcessBeforeEventTime
    ============================================================
    PURPOSE: Verify event doesn't execute if you process before its scheduled time
    ============================================================
*/
TEST(LatencyQueueTest, ProcessBeforeEventTime) {
    
}

/**
    ============================================================
    TEST 7: IsEmptyWorks
    ============================================================
    PURPOSE: Verify is_empty() returns correct state throughout lifecycle
    ============================================================
*/
TEST(LatencyQueueTest, IsEmptyWorks) {
    
}

/**
    ============================================================
    TEST 8: AllActionTypes
    ============================================================
    PURPOSE: Test that all 5 ActionTypes produce latency within their bounds
    ============================================================
*/
TEST(LatencyQueueTest, AllActionTypes) {
    
}