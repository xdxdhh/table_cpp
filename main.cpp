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
    Table t;
    t.add_col("vek","Int");
    t.add_cols({"pohlavi", "Bool", "jmeno", "String"});

    t.add_record(Int(20), Bool(false), String("Martin"));
    t.add_record(Int(20), Bool(false), Blank());
    //t.add_record(put<Int>(20), put<Bool>(false), put<String>("Martin"));
    //t.add_record(put<Int>(60), put<Blank>(), put<String>("Martin"));
    //t.add_record(put<Int>(40), put<Bool>(true) , put<String>("Agata"));

    t.print();
    t.describe();
    t.delete_col("id");
    t.print();
    t.describe();

    //auto d = t.find("jmeno", String("Martin")).find("vek", Int(20)) ;
    //d.print();

}

