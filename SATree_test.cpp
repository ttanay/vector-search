#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "SATree.h"

enum TestCaseStatus
{
    PASS,
    FAIL
};
struct TestCaseResult
{
    std::string name;
    TestCaseStatus status;
    std::string err;

    void display()
    {
        if (status == TestCaseStatus::PASS)
            std::cout << "TestCase \"" << name << "\": PASSED" << std::endl;
        else
            std::cout << "TestCase \"" << name << "\": FAILED with " << err << std::endl;
    }
};

TestCaseResult test_case1(const std::string & name)
{
    Points<2> input{{3, 3}, {5, 3}, {2, 2}, {4, 4}};
    SATree<2> tree(input);
    std::string res = tree.to_string();
    /**
        |- (4, 4) |N(a)| = 2
            |- (3, 3) |N(a)| = 1
                |- (2, 2) |N(a)| = 0
            |- (5, 3) |N(a)| = 0
    */
    std::string expected = "{(4.000000, 4.000000)}({(3.000000, 3.000000)}({(2.000000, 2.000000)}),{(5.000000, 3.000000)})";
    if (res == expected)
        return {name, TestCaseStatus::PASS, ""};
    else
    {
        auto err = "Expected: " + expected + "; Got: " + res;
        return {name, TestCaseStatus::FAIL, err};
    }
}

TestCaseResult test_case2(const std::string & name)
{
    Points<2> input{{-3, -3}, {-3, -4}, {-5, -3}, {-4, -4}, {-1, -1}};
    SATree<2> tree(input);
    auto res = tree.to_string();
    // Note: the {-5,-3} should be a child of {-3,-3} because it is the
    //  closest to the pivot(`a`) at that point in time
    /**
    |- (-1, -1) |N(a)| = 1
        |- (-3, -3) |N(a)| = 2
            |- (-3, -4) |N(a)| = 1
                |- (-4, -4) |N(a)| = 0
        |- (-5, -3) |N(a)| = 0

    */
    std::string expected = "{(-1.000000, -1.000000)}({(-3.000000, -3.000000)}({(-3.000000, -4.000000)}({(-4.000000, -4.000000)}),{(-5.000000, -3.000000)}))";
    if (res == expected)
        return {name, TestCaseStatus::PASS, ""};
    else
    {
        auto err = "Expected: " + expected + "; Got: " + res;
        return {name, TestCaseStatus::FAIL, err};
    }
}

TestCaseResult test_case3(const std::string & name)
{
    Points<2> input{{-3, -3}, {-3, -4}, {-5, -3}, {-4, -4}, {-1, -1}};
    SATree<2> tree(input);
    auto res = tree.range_search({-6, -2}, 2.0);
    Point<2> expected{-5, -3};
    if (res && res.value() == expected)
    {
        return {name, TestCaseStatus::PASS, ""};
    }
    else if (!res)
        return {name, TestCaseStatus::FAIL, "returned null"};
    else
    {
        std::string err = "Expected: " + expected.to_string() + "; Got: " + res->to_string();
        return {name, TestCaseStatus::FAIL, err};
    }
}

TestCaseResult test_case4(const std::string & name)
{
    Points<2> input{{-3, -3}, {-3, -4}, {-5, -3}, {-4, -4}, {-1, -1}};
    SATree<2> tree(input);
    auto res = tree.range_search({-6, -2}, 1.0);
    Point<2> expected{-5, -3};
    if (!res)
    {
        return {name, TestCaseStatus::PASS, ""};
    }
    else
    {
        std::string err = "Expected: " + expected.to_string() + "; Got: " + res->to_string();
        return {name, TestCaseStatus::FAIL, err};
    }
}

TestCaseResult test_case5(const std::string & name)
{
    Points<2> input{{1, 1}, {3, 3}, {5, 3}, {3, 4}, {6, 4}, {-3, -3}, {-3, -4}, {-5, -3}, {-4, -4}, {-1, -1}};
    SATree<2> tree(input);
    auto res = tree.nearest_neighbour_search({-4, -5}, 2);

    kNNResult<2> expected{{{-3, -4}, (float)std::sqrt(2.0)}, {{-4, -4}, 1.0}};

    if (res.size() != 2)
    {
        std::string err = "Expected size: 2; Got: " + std::to_string(res.size());
        return {name, TestCaseStatus::FAIL, err};
    }

    if (res != expected)
    {
        std::string err = "Expected: [";
        for (auto i : expected)
            err += i.to_string();
        err += "]; Got: [";
        for (auto i : res)
            err += i.to_string();
        err += "]";
        return {name, TestCaseStatus::FAIL, err};
    }

    return {name, TestCaseStatus::PASS, ""};
}

TestCaseResult test_case6(const std::string &name)
{
    Points<2> input{{1, 1}, {3, 3}, {5, 3}, {3, 4}, {6, 4}, {-3, -3}, {-3, -4}, {-5, -3}, {-4, -4}, {-1, -1}};
    SATree<2> tree(input);
    if (!tree.nearest_neighbour_search({-4, -5}, 0).empty())
    {
        std::string err = "Expected empty result";
        return {name, TestCaseStatus::FAIL, err};
    }
    return {name, TestCaseStatus::PASS, ""};
}

TestCaseResult test_case7(const std::string &name)
{
    Points<3> input{{1, 1, 1}, {3, 3, 3}, {5, 3, 2}, {3, 4, 1}, {6, 4, 2}};
    SATree<3> tree(input);
    auto res = tree.nearest_neighbour_search({1, 1, 0}, 1);

    kNNResult<3> expected{{{1, 1, 1}, 1.0}};

    if(res.size() != 1)
        return {name, TestCaseStatus::FAIL, "Result does not have 1 element"};

    if (res != expected)
    {
        std::string err = "Expected: [";
        for (auto i : expected)
            err += i.to_string();
        err += "]; Got: [";
        for (auto i : res)
            err += i.to_string();
        err += "]";
        return {name, TestCaseStatus::FAIL, err};
    }

    return {name, TestCaseStatus::PASS, ""};
}

int main(int argc, char ** argv)
{
    test_case1("Basic tree construction").display();
    test_case2("Skipped neighbour").display();
    test_case3("Range search").display();
    test_case4("Range search null").display();
    test_case5("kNN Search").display();
    test_case6("kNN Search empty").display();
    test_case7("kNN 3-dimensions").display();
    return 0;
}