#include <thread>
#include <mutex>
#include <condition_variable>

int soup_servings = 10;
std::mutex slow_cooker_lid;
std::condition_variable soup_taken;
void hungry_person(int id)
{
        int put_lid_back = 0;
        while(soup_servings > 0)
        {
                std::unique_lock<std::mutex> lid_lock(slow_cooker_lid);
                while((id!=soup_servings%5) && (soup_servings > 0 ))
                {
                        put_lid_back++;
                        soup_taken.wait(lid_lock);
                /*      soup_servings--; */
                }
                if(soup_servings>0){
                        soup_servings--;
                        lid_lock.unlock();
                        soup_taken.notify_all();
                }
        }
        printf("Person %d put the lid back %u times.\n",id, put_lid_back);

}

int main()
{
        std::thread hungry_threads[5];
        for(int i = 0;i<5;i++)
        {
                hungry_threads[i] = std::thread(hungry_person,i);
        }
        for(auto& ht: hungry_threads)
        {
                ht.join();
        }
        return 0;
}