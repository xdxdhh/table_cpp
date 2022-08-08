# Table Class

## 1. Table functions


__Table t(name);__   --initializes an empty table with a given name

-__t.add_col(name of column, type)__ --adds one column                                             
Column of blanks cannot be created  

-__t.add_cols(names of columns types of cols)__ --adds multiple columns
example: t.add_cols({"sex", "Bool", "name", "String"});  
allowed types(see 4.Data types) are checked when adding column  

-__t.delete_col(column_name)__ -- deletes one col from all records 

-__t.delete_cols()__ --   delete multiple columns 
example: t.delete_cols({"name","age","color"});

-__t.rename_col()__  -- rename one column

-__t.get_cols()__ --returns vector with names of columns  

-__t.get_coltypes()__ --return vector with type names of columns  

-__t.truncate()__ --changes indexes so that they form integer sequence starting with 1 

-__t.print()__ --prints a 'pretty formatted' table into console 

-__t.describe()__ --show some basic information about table, as number of columns,rows, column name and types  




## 2.Record functions 

-__t.add_record(arbitrary number of Data class objects)__ --adding one record \
-example: t.add_record(put(Int,20), put(Bool,false), put(String, "Martin")); \
-when adding a record, types of Data should be checked with col_types;  

When adding a record, the number of arguments and number of columns(not indluding index column) should be matching. \
For adding a blank data use Blank data type.  

-__t.delete_record(column_name, data)__ --deletes all records which contain data in a given column \
For deleting just one record, ids can be used, e.g. t.delete_record("id", 2);  

-__t.clear_records()__ --deletes all records, leaves an empty table with cols  



## 3. Working with multiple tables

-__t.find(column_name, data)__  returns Table with all of the records having data in that column \
Find can be chained, e.g: auto d = t.find("name", String("Martin")).find("age", Int(20));  


## 4. Data types structure

Currently there are 4 pre-implemented types of Data and those are Integer, String, Boolean and Blank. \
User can define his own type, which has to contain: \
1.type() function, which returns name of type, e.g. "Int", "Bool" \
2.to_str() function, some visual representation of data which allows the whole table to be printed into console\
3.to_bytes() function, that transform the data into bytes reprezentation(vector of bytes)\
4.clone() function, which returns a pointer to clone of the data  

When adding data to table, correctness of types is checked, Blank data type can be used in column of any type.  


### 5. JSON structure, loading and saving tables

Example JSON input/output structure:


```
{
    "columns_class": {
        "cols": [
            {
                "name": "Jmeno", 
                "primary_key_flag": false, 
                "type": "String"
            }, 
            {
                "name": "Vek", 
                "primary_key_flag": false, 
                "type": "Int"
            }, 
            {
                "name": "Pohlavi", 
                "primary_key_flag": false, 
                "type": "Bool"
            }
        ], 
        "foreign_keys": {}, 
        "number_of_cols": 3
    }, 
    "name": "lidi", 
    "records_class": {
        "number_of_recs": 2, 
        "records": [
            {
                "data": [
                    {
                        "data": "SmFuYQ==", 
                        "type": "String"
                    }, 
                    {
                        "data": "FAAAAA==", 
                        "type": "Int"
                    }, 
                    {
                        "data": "AA==", 
                        "type": "Bool"
                    }
                ], 
                "index": 1, 
                "num_of_data": 3
            }, 
            {
                "data": [
                    {
                        "data": "Sm9zZWY=", 
                        "type": "String"
                    }, 
                    {
                        "data": "LAAAAA==", 
                        "type": "Int"
                    }, 
                    {
                        "data": "AQ==", 
                        "type": "Bool"
                    }
                ], 
                "index": 2, 
                "num_of_data": 3
            }
        ]
    }
}

```



