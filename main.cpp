#include "table.h"
#include "iostream"
#include <vector>
#include "jsoncons/include/jsoncons/json.hpp"
#include <fstream>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>
#include <jsoncons_ext/bson/bson.hpp>
#include <map>


using namespace std;
using namespace jsoncons;
using jsoncons::jsonpath::json_query;
namespace bson = jsoncons::bson;


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

    /* Table t("human");
    t.add_cols({"name", "String", "age", "Int", "sex", "Bool"});
    t.add_record(String("Anna"), Int(20), Bool(false));
    t.add_record(String("Anna"), Int(20), Bool(true));

    t.print();
    t.describe();
    t.delete_col("age");
    t.print();
    t.delete_record("sex", Bool(false));
    t.print();
    t.add_record(String("Martin"), Blank());
    t.print();
    t.truncate();
    t.print(); */
    /* t.rename_col("name", "jmeno");
    t.print();
    t.describe();

    t.delete_cols({"age", "sex"});
    t.print(); 
 */
    //std::string s = R"({"first":1,"second":2,"fourth":3,"fifth":4})";   

    //cols: name age sex
    /* json table1;
    table1["name"] = "tablename";
    std::vector<string> cols;
    cols.push_back("age");
    cols.push_back("name");
    cols.push_back("sex");
    std::vector<string> coltypes;
    coltypes.push_back("int");
    coltypes.push_back("string");

    table1["cols"] = cols;
    table1["coltypes"] = coltypes;
    
    cout << table1 << endl;

    std::ofstream os("test.json");
    os << pretty_print(table1);

    std::ifstream is("test.json");
    json j = json::parse(is); 
    cout << j << endl;
    std::vector<string> cols_new;
    cout << j["cols"] << endl;
    //auto it = j.find("name");
    auto it = j.get_with_default("name", "name unknown");
    cout << it << endl;
    auto x = j.at_or_null("cols");

    for(auto it = x.array_range().begin() ; it != x.array_range().end(); it++){
        cout << (*it).as<string>() << endl;
    }

    /* cout << x.at(0) << endl;
    string col1 = x.at(0).as<string>();
    cout << col1 << endl; */
    
    //std::vector<std::unique_ptr<Data>> contents;
    /* json record1;
    record1["index"] = 1;
    

    auto hodnota = Int(513).to_bytes();
    std::vector<uint8_t> buffer;
    bson::bson_bytes_encoder encoder(buffer); */
    //encoder.begin_object(); // The total number of bytes comprising 
                            // the bson document will be calculated
    //encoder.key("Int");
    //std::vector<uint8_t> bstr = {'f','o','o','b','a','r'};
    //encoder.byte_string_value(hodnota); // default subtype is user defined
    // or encoder.byte_string_value(bstr, 0x80); 
/*     encoder.end_object();
    encoder.flush();

    std::cout << "(1)\n" << jsoncons::byte_string_view(buffer) << "\n";
 */
    //std::ofstream os("test.json");
/*     ojson hh = bson::decode_bson<ojson>(buffer);
    ojson test;
    test["Data"] = hh;

    cout << test << endl;
     */

    //get type:
    
/*     auto vec = hh["Int"].as<std::vector<uint8_t>>();
    
    for (auto & v : vec) cout << (int)v << endl;
    cout << Data::copy_from_bytes<Int>(vec).to_str() << endl; */
    Table t("lidi");
    t.add_cols({"Jmeno", "String",  "Vek", "Int", "Pohlavi", "Bool"});
    t.add_record(String("Jana"), Int(20), Bool(false));
    t.add_record(String("Josef"), Int(44), Bool(true));
    t.serialize(); //TBD dat option vybrat si vlastni jmeno souboru 
    Table s("table_lidi.json", "json");
    if(t == s)
        cout << "yy.";
    t.describe(); //porovnavame obsahy ne nazvy
    s.describe();
    cout << t << endl;
    cout << s << endl;
    
    
 
}

