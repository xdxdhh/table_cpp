
#include "table.h"
#include "iostream"
#include <vector>
#include <gtest/gtest.h>

using namespace std;


// Demonstrate some basic assertions.
TEST(ColumnTests, BasicAssertions) {
    Table t;
    t.add_col("vek","Int");
    t.add_col("jmeno","String");
    std::vector<std::string> vec = {"Int", "Int", "String"};
    auto cols = t.get_coltypes();
    
    EXPECT_EQ(cols.size(),vec.size());

    for(auto i = 0; i < vec.size();i++){
        EXPECT_TRUE(cols.at(i) == vec.at(i)) << "MSG: iteration " << i << "   "<< cols.at(i) << "not equal" << vec.at(i);
    }
    //adding already existing col should trow:
    EXPECT_THROW(t.add_col("vek","Int"), std::invalid_argument);
    EXPECT_THROW(t.add_col("id","String"), std::invalid_argument);
    //adding anoter id column should throw 
}

TEST(BasicFunctionality, Columnns) {
    Table t;
    Table d;
    EXPECT_TRUE(t == d);
    EXPECT_EQ(t.get_col_num(), 1);
    EXPECT_EQ(t.get_row_num(), 0);
    t.add_cols({"name", "String", "age", "Int", "sex", "Bool"});
    EXPECT_THROW(t.add_col("hah", "Blank"), std::invalid_argument);
    EXPECT_THROW(t.add_col("sex", "String"), std::invalid_argument);
    EXPECT_THROW(t.add_col("sex", "Bool"), std::invalid_argument);
    EXPECT_THROW(t.add_col("sex", "bool"), std::invalid_argument);
    //EXPECT_THROW(t.add_col("Sex", "bool"), std::invalid_argument);   TBD SLOUPECKY ABY NEBYLY CASE SENSITIVE

}

TEST(BasicFunctionality, Records){
    Table t;
    t.add_cols({"name", "String", "age", "Int", "sex", "Bool"});
    t.add_record(String("Anna"), Int(20), Bool(true));
    t.add_record(String("Anna"), Int(20), Bool(true));
    EXPECT_THROW(t.add_record(String("Anna"), Int(20), Bool(true), String("A")), std::invalid_argument);
    //EXPECT_THROW(t.add_record(Blank(), Blank(), Blank()), std::invalid_argument);  //TBD NELZE UDELAT JEN blank radek
    t.add_record(Blank(), Int(40), Bool(true));
    //EXPECT_THROW(t.add_record(Int(25)), std::invalid_argument);
}

TEST(AdvancedFunctionality, Find){
    Table t;
    t.add_cols({"name", "String", "age", "Int", "sex", "Bool"});
    t.add_record(String("Anna"), Int(20), Bool(true));
    t.add_record(String("Anna"), Int(20), Bool(true));
    t.add_record(String("Anna"), Int(20), Bool(true));
    auto d = t.find("name", String("Anna"));
    EXPECT_TRUE(t == d);

    Table empty;
    auto ctable = t.find("name", String("Bob")); //should return an empty table
    ctable.delete_col("name");
    ctable.delete_col("age");
    ctable.delete_col("sex"); 

    EXPECT_TRUE(empty == ctable) << "Tables" << empty << " " << ctable << "are not equal.";
    t.delete_record("age", Int(20)); //should be empty aswell
    t.delete_col("name");
    t.delete_col("age");
    t.delete_col("sex");
    EXPECT_TRUE(empty == t) << "Tables" << empty << " " << t << "are not equal.";
    EXPECT_THROW(t.add_record(String("Tom"), Int(66), Bool(false)), std::invalid_argument);
    EXPECT_THROW(auto y = t.find("name", String("Tom")), std::invalid_argument);

    //y.print();
    //should be equal:
    //Table z;
    //z.add_cols({"name", "String", "age", "Int", "sex", "Bool"});
    //z.add_record(String("Tom"), Int(66), Bool(false));
    //z.print();
    //EXPECT_TRUE(y == z);                          //POROVNITKO NESMI BRAT V POTAZ ID THINK

}