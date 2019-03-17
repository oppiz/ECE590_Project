#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "gtest/gtest.h"
#include "elma.h"

namespace {

    using namespace elma;
    using std::vector;

    const int Priority_min = -5, Priority_max = 15; 
    static int Fast_ADDIT = 0;
    static int Slow_ADDIT = 0;
    

    class  SlowMulti: public elma::Process {

     public: 
        SlowMulti(string name) : Process(name, 1, MULTI) {}
        void init() {}
        void start() {}
        void update() {
            usleep(30000);
            _mtx.lock();
            Slow_ADDIT++;
            _mtx.unlock();
            
        }
        void stop() {}
        private:
        std::mutex _mtx;

    };    

    class  SlowSingle: public elma::Process {

    public: 
        SlowSingle(string name) : Process(name, 1, SINGLE) {}
        void init() {}
        void start() {}
        void update() {
            usleep(30000);
            Slow_ADDIT++;
        }
        void stop() {}

    };  

    class  Fast: public elma::Process {

    public: 
        Fast(string name) : Process(name) {}
        void init() {}
        void start() {}
        void update() {
            Fast_ADDIT++;
        }
        void stop() {}

    };    

    TEST(Singlethread, basic) {              
    
        elma::Manager m;
        SlowSingle SLEEP("SLEEPSingle");
        Fast count("count");
        
        m.schedule(SLEEP, 20_ms)
         .schedule(count, 3_ms)
         .init()
         .run(91_ms);
        //std::cout << "SINGLE Fast_ADDIT: " << Fast_ADDIT << " Slow_ADDIT : " << Slow_ADDIT << "\n";
        
    }

    TEST(Multithread, basic) {              
        Fast_ADDIT = 0;
        Slow_ADDIT = 0;

        elma::Manager m;
        SlowMulti SLEEPS("SLEEPMulit");
        Fast count("count");
        
        m.schedule(SLEEPS, 20_ms)
         .schedule(count, 3_ms)
         .init()
         .run(91_ms);

        //Give time for everything to finish             
        sleep (1);
        //std::cout << "MULTI Fast_ADDIT: " << Fast_ADDIT << " Slow_ADDIT : " << Slow_ADDIT << "\n";
        EXPECT_EQ(Fast_ADDIT, 30);
        EXPECT_EQ(Slow_ADDIT, 4);
        
    }

    


}

