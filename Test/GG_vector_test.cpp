#include "GG_vector_test.h"
#include "GG_test_utils.h"
#include <cassert>

__GG_BEGIN_NAMESPACE

namespace GGTest {
    void testVectorCase1() {
        std::vector<std::string> v1(10,"kzw");
        vector<std::string> v2(10,"kzw");
        assert(container_equal(v1,v2));
        std::vector<std::string> v3(10);
        vector<std::string> v4(10);
        assert(container_equal(v3,v4));
        std::array<std::string,3> arr = {"abc","def","ghi"};
        std::vector<std::string> v5(std::begin(arr),std::end(arr));
        vector<std::string> v6(std::begin(arr),std::end(arr));
        assert(container_equal(v5,v6));
    }

    void testVectorCase2(){
        ;
    }
    void testVectorCase3(){
        ;
    }
    void testVectorCase4(){
        ;
    }
    void testVectorCase5(){
        ;
    }
    void testVectorCase6(){
        ;
    }
    void testVectorCase7(){
        ;
    }
    void testVectorCase8(){
        ;
    }
    void testVectorCase9(){
        ;
    }
    void testVectorCase10(){
        ;       
    }
    void testVectorCase11(){
        ;        
    }
    void testVectorCase12(){
        ;     
    }
    void testVectorCase13(){
        ;  
    }
    void testVectorCase14(){
        ; 
    }

    void testVectorAllCases() {
        testVectorCase1();
        testVectorCase2();
        testVectorCase3();
        testVectorCase4();
        testVectorCase5();
        testVectorCase6();
        testVectorCase7();
        testVectorCase8();
        testVectorCase9();
        testVectorCase10();
        testVectorCase11();
        testVectorCase12();
        testVectorCase13();
        testVectorCase14();
    }
}

__GG_END_NAMESPACE

int main(){
    gadget::GGTest::testVectorAllCases();
}