#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class ProducerConsumerNode : public rclcpp::Node{
    public:

        ProducerConsumerNode()
            : Node("producer_consumer_node"),
            running_(true){
            RCLCPP_INFO(
                this->get_logger(),
                "Producer/Consumer demo started"
            );

            producer_thread_ =
                std::thread(
                    &ProducerConsumerNode::producer_loop,
                    this
                );

            consumer_thread_ =
                std::thread(
                    &ProducerConsumerNode::consumer_loop,
                    this
                );
        }

        ~ProducerConsumerNode(){
            running_ = false;

            condition_.notify_all();
            if (producer_thread_.joinable()){
                producer_thread_.join();
            }

            if (consumer_thread_.joinable()){
                consumer_thread_.join();
            }
            RCLCPP_INFO(
                this->get_logger(),
                "Threads stopped"
            );
        }

    private:
        void producer_loop(){
            int frame_id = 0;

            while (running_){
                {
                    std::lock_guard<std::mutex> lock(
                        mutex_
                    );

                    queue_.push(frame_id);

                    RCLCPP_INFO(
                        this->get_logger(),
                        "[PRODUCER] Produced frame %d",
                        frame_id
                    );

                    ++frame_id;
                }

                condition_.notify_one();
                std::this_thread::sleep_for(500ms);
            }
        }

        void consumer_loop(){
            while (running_){
                int frame = -1;
                {
                    std::unique_lock<std::mutex> lock(
                        mutex_
                    );
                    condition_.wait(
                        lock,
                        [this](){
                            return !queue_.empty()
                                || !running_;
                        }
                    );

                    if (!running_ && queue_.empty()){
                        break;
                    }

                    frame = queue_.front();

                    queue_.pop();
                }

                RCLCPP_INFO(
                    this->get_logger(),
                    "[CONSUMER] Processing frame %d",
                    frame
                );
                std::this_thread::sleep_for(1s);
            }
        }

    private:
        std::queue<int> queue_;
        std::mutex mutex_;
        std::condition_variable condition_;
        std::thread producer_thread_;
        std::thread consumer_thread_;
        bool running_;
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node =
        std::make_shared<ProducerConsumerNode>();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}