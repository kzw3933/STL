#include <GG_vector_test.h>

__GG_BEGIN_NAMESPACE

namespace VectorTest {
    void testCase1() {
        stdVec<std::string> v1(10,"kzw");
        tsVec<std::string> v2(10,"kzw");
        assert(Test::container_equal(v1,v2));

        stdVec<std::string> v3(10);
        tsVec<std::string> v4(10);
        assert(Test::container_equal(v3,v4));

        std::array<std::string,3> arr = {"abc","def","ghi"};
        stdVec<std::string> v5(std::begin(arr),std::end(arr));
        tsVec<std::string> v6(std::begin(arr),std::end(arr));
        assert(Test::container_equal(v5,v6));

    }

    void testCase2(){

    }
    void testCase3(){

    }
    void testCase4(){

    }
    void testCase5(){

    }
    void testCase6(){

    }
    void testCase7(){

    }
    void testCase8(){

    }
    void testCase9(){

    }
    void testCase10(){
        
    }
    void testCase11(){
        
    }
    void testCase12(){
        
    }
    void testCase13(){
        
    }
    void testCase14(){
        
    }

    void testAllCases() {
        testCase1();
        testCase2();
        testCase3();
        testCase4();
        testCase5();
        testCase6();
        testCase7();
        testCase8();
        testCase9();
        testCase10();
        testCase11();
        testCase12();
        testCase13();
        testCase14();
    }
}

__GG_END_NAMESPACE