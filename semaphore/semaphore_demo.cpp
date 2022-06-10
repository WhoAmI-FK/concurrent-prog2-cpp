#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

class Semaphore
{
        public:
                typedef unsigned long ul;
                Semaphore(ul init_count){
                        _count = init_count;
                }

                void acquire()
                {
                        std::unique_lock<std::mutex> lck(_m);
                        while(!_count)
                        {
                                _cv.wait(lck);
                        }
                        _count--;
                }
                void release()
                {
                        std::unique_lock<std::mutex> lck(_m);
                        _count++;
                        lck.unlock();
                        _cv.notify_one();
                }
        private:
                ul _count;
                std::mutex _m;
                std::condition_variable _cv;
};

Semaphore charger(1);

void cell_phone(int id)
{
        charger.acquire();
        printf("Phone %d is charging...\n",id);
        srand(id);
        std::this_thread::sleep_for(std::chrono::milliseconds(rand()%2000 + 1000));
        printf("Phone %d is DONE charging!\n",id);
        charger.release();
}

int main()
{
        std::thread phones[10];
        for(int i = 0;i<10;i++)
        {
                phones[i] = std::thread(cell_phone,i);
        }
        for(auto& p: phones)
        {
                p.join();
        }
}