#include <thread>
#include <mutex>
//#include <boost/thread/barrier.hpp
#include <boost/thread/latch.hpp>
unsigned int bags_of_chips = 1;
std::mutex pencil;
//boost::barrier fist_bump(10);
boost::latch fist_bump(5);
void cpu_work(unsigned long workUnits)
{
        unsigned long x = 0;
        for(unsigned long i; i<workUnits*1000000;i++)
        {
                x++;
        }
}

void barron_shopper()
{
        cpu_work(1);
        fist_bump.wait();
        std::scoped_lock<std::mutex> lock(pencil);
        bags_of_chips*=2;
        printf("Barron DOUBLED the bags of chips.\n");
}

void olivia_shopper()
{
        cpu_work(1);
        {
        std::scoped_lock<std::mutex> lock(pencil);
        bags_of_chips +=3;
        }
        printf("Olivia ADDED 3 bags of chips.\n");
//      fist_bump.wait();
        fist_bump.count_down();
}

int main()
{
        std::thread shoppers[10];
        for(int i = 0;i<10;i+=2){
                shoppers[i] = std::thread(barron_shopper);
                shoppers[i+1] = std::thread(olivia_shopper);
        }

        for(auto& s: shoppers)
        {
                s.join();
        }
        printf("We need to buy %u bags_of_chips.\n",bags_of_chips);
        return 0;
}