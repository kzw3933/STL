#ifndef __GG_VECTOR_TEST_H
#define __GG_VECTOR_TEST_H

#include <vector>
#include <string>
#include <array>
#include <cassert>
#include <GG_test_utils.h>
#include <../GG_config.h>
#include <../GG_vector.h>

__GG_BEGIN_NAMESPACE

namespace VectorTest {
    template <class T>
    using stdVec = std::vector<T>;

    template<class T>
    using tsVec = vector<T>;

    void testCase1();
    void testCase2();
    void testCase3();
    void testCase4();
    void testCase5();
    void testCase6();
    void testCase7();
    void testCase8();
    void testCase9();
    void testCase10();
    void testCase11();
    void testCase12();
    void testCase13();
    void testCase14();

    void testAllCases();
}

__GG_END_NAMESPACE
#endif