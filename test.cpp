
#include "table.h"
#include "iostream"
#include <vector>
#include <gtest/gtest.h>

using namespace std;


// Demonstrate some basic assertions.
TEST(ColumnTests, BasicAssertions) {
    Table t("people");
    t.add_col("age","Int");
    t.add_col("name","String");
    std::vector<std::string> vec = {"Int", "String"};
    auto cols = t.get_coltypes();
    
    EXPECT_EQ(cols.size(),vec.size());

    for(auto i = 0; i < vec.size();i++){
        EXPECT_TRUE(cols.at(i) == vec.at(i)) << "MSG: iteration " << i << "   "<< cols.at(i) << "not equal" << vec.at(i);
    }
    //adding already existing col should trow:
    EXPECT_THROW(t.add_col("age","Int"), std::invalid_argument);
    //deleting non-existing column should throw:
    EXPECT_THROW(t.delete_col("non-existing"), std::invalid_argument);
    //deleting non-existing column should throw:
    EXPECT_THROW(t.delete_cols({"age", "non-existing"}), std::invalid_argument);


}

TEST(BasicFunctionality, Columns) {
    Table t("table1");
    Table d("table2");
    //shoule be equal(empty):
    EXPECT_TRUE(t == d);
    EXPECT_EQ(t.get_col_num(), 0);
    EXPECT_EQ(t.get_row_num(), 0);
    t.add_cols({"name", "String", "age", "Int", "sex", "Bool"});
    //blank column cannot be added
    EXPECT_THROW(t.add_col("hah", "Blank"), std::invalid_argument);
    //already existing column cannot be added
    EXPECT_THROW(t.add_col("sex", "String"), std::invalid_argument);
    EXPECT_THROW(t.add_col("sex", "Bool"), std::invalid_argument);
    //column names should be not be case sensitive:
    EXPECT_THROW(t.add_col("Sex", "Bool"), std::invalid_argument);
    d.add_cols({"name", "String", "age", "Int", "gender", "Bool"});
    EXPECT_FALSE(t == d);
    d.rename_col("gender", "bool");
    //tables should be equal after rename:
    EXPECT_TRUE(t == d);


}

TEST(BasicFunctionality, Records){
    Table t("test3");
    t.add_cols({"name", "String", "age", "Int", "sex", "Bool"});
    t.add_record(String("Anna"), Int(20), Bool(true));
    t.add_record(String("Anna"), Int(20), Bool(true));
    //record with not matching number of Data should not be added
    EXPECT_THROW(t.add_record(String("Anna"), Int(20), Bool(true), String("A")), std::invalid_argument);
    EXPECT_THROW(t.add_record(Int(25)), std::invalid_argument);
    //record full of Blanks cannot be added:
    EXPECT_THROW(t.add_record(Blank(), Blank(), Blank()), std::invalid_argument); 
    //column with one Blank can be added
    t.add_record(Blank(), Int(40), Bool(true));
    
}

/*
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
    ctable.delete_col("sex"); //ok

    EXPECT_TRUE(empty == ctable) << "Tables" << empty << " " << ctable << "are not equal.";
    t.delete_record("age", Int(20)); //should be empty aswell
    t.delete_col("name");
    t.delete_col("age");
    t.delete_col("sex");
    EXPECT_TRUE(empty == t) << "Tables" << empty << " " << t << "are not equal.";
    EXPECT_THROW(t.add_record(String("Tom"), Int(66), Bool(false)), std::invalid_argument);
    EXPECT_THROW(auto y = t.find("name", String("Tom")), std::invalid_argument);

}*/