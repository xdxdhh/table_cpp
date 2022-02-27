#include "table.h"
#include "iostream"
#include <vector>

using namespace std;

void print(Data &d)
{

}

//#define put(type, value) std::unique_ptr<Data>(dynamic_cast<Data*>(new type(value)))
//#define blank std::unique_ptr<Data>(dynamic_cast<Data*>(new Blank()))


int main(int argc, char const *argv[])
{
    /* Table t;
    t.add_col("vek","Int");
    t.add_cols({"pohlavi", "Bool", "jmeno", "String"});

    t.add_record(Int(20), Bool(false), String("Martin")); */
    //t.add_record(Int(20), Bool(false), Blank());
    //t.add_record(put<Int>(20), put<Bool>(false), put<String>("Martin"));
    //t.add_record(put<Int>(60), put<Blank>(), put<String>("Martin"));
    //t.add_record(put<Int>(40), put<Bool>(true) , put<String>("Agata"));

    /* cout << t << endl;
    
    Table x;
    x.add_col("vek","Int");
    x.add_cols({"pohlavi", "Bool", "jmeno", "String"});
    x.add_record(Int(20), Bool(false), String("Marsstin"));
    x.add_record(Int(20), Bool(false), String("X"));
    x.add_record(Int(20), Bool(false), String("Y"));


    x.delete_record("jmeno", String("X"));
    x.print();
    x.truncate();
    x.print(); */
    //auto d = t.find("jmeno", String("Martin")).find("vek", Int(20)) ;
    //d.print();

    Table t;
    t.add_cols({"name", "String", "age", "Int", "sex", "Bool"});
    t.add_record(String("Anna"), Int(20), Bool(true));
    t.add_record(String("Anna"), Int(20), Bool(true));

    auto ctable = t.find("name", String("Bob")); //should return an empty table
    cout << "printing ctable: " << ctable << endl;
    ctable.describe();
    ctable.delete_col("name");
    cout << ctable << endl;
    ctable.describe();
    ctable.delete_col("age");
    cout << ctable << endl;
    ctable.delete_col("sex");
    cout << ctable << endl;

}

