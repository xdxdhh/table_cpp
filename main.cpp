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


    t.add_record(put<Int>(20), put<Bool>(false), put<String>("Martin"));
    t.add_record(put<Int>(20), put<Blank>(), put<String>("Martin"));
    //t.add_record(put(Int,40), blank , put(String, "Martin"));
    //t.add_record(put(Int,30), put(Bool,true), put(String, "Diana"));

    t.print();
    //t.clear_records();
    //t.delete_record("vek", Int(40));
    //t.print();
    //t.truncate();
    //t.print();
    //t.clear_records();
    //t.print();

    

    //t.delete_record("jmeno", String("Martidsan"));
    
    /* Table d = t.find("jmeno", String("Martin"));
    t.print();
    cout << "---" << endl;
    d.print();

    t.add_col("barva", "String");
    t.print();

    t.delete_record("pohlavi", Bool(false));
    t.print(); */

/*     t.print();
    t.describe();
    cout << "deleting record " << endl;
    t.delete_record("vek", Int(20));
    t.print();
    t.describe();
    t.add_record(put(Int,11), put(Bool,true), put(String, "Renne"));
    t.add_col("cislo", "Int");
    t.print();
    t.describe();
    t.add_record(put(Int,11), put(Bool,true), put(String, "Renne"), put(Int, 22));

 */


}

